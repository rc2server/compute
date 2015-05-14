#include "FileManager.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <event2/bufferevent.h>
#include <postgresql/libpq-fe.h>
#include <glog/logging.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <sys/types.h>
#include <utime.h>
#include "../common/FormattedException.hpp"

using namespace std;
using boost::format;
namespace fs = boost::filesystem;

#undef LOG
#define LOG(x) cerr

class DBResult {
	PGresult *res;
	public:
		DBResult(PGresult *inRes) : res(inRes) {}
		~DBResult() { PQclear(res); }
		operator PGresult*() const { return res; }
};

//for db fetched data
struct DBFileInfo {
	long		id, version;
	string		name, path;
	int			watchDescriptor;
	struct stat	sb;
	bool		projectFile;
	
	DBFileInfo(uint32_t anId, uint32_t aVersion, string &aName, bool projFile=false)
		: id((long)anId), version((long)aVersion), name(aName), projectFile(projFile)
	{
		if (projectFile)
			path = "shared/";
		path += name;
	}
	
	friend ostream& operator<<(ostream &out, DBFileInfo &finfo) {
		out << "(" << finfo.id << ": " << finfo.name << ")";
		return out;
	}
};

typedef shared_ptr<DBFileInfo> DBFileInfoPtr;

struct FSDirectory {
	int wd;
	struct stat sb;
};

class RC2::FileManager::Impl {
	public:
		long						wspaceId_;
		long						projectId_;
		long						sessionRecId_;
		PGconn*						dbcon_;
		map<long, DBFileInfoPtr>	filesById_;
		map<int, DBFileInfoPtr>		filesByWatchDesc_;
		vector<long>				imageIds_;
		string						workingDir;
		struct event_base*			eventBase_;
		int							inotifyFd_;
		FSDirectory					rootDir_;
		boost::regex				imgRegex_;

		Impl()
			: imgRegex_("rc2img(\\d+).png")
		{}

		void cleanup(); //replacement for destructor
		void connect(string str, long wspaceId, long sessionRecId);
		void loadFiles(const char *whereClause, bool isProject);
		long insertDBFile(string fname); //returns db id
		void updateDBFile(DBFileInfoPtr fobj);
		void removeDBFile(DBFileInfoPtr fobj);
		long query1long(const char *query);
		
		void	setupInotify(FileManager *fm);
		void 	watchFile(DBFileInfoPtr file);
		void	handleInotifyEvent(struct bufferevent *bev);
		static void handleInotifyEvent(struct bufferevent *bev, void *ctx)
		{
			FileManager *watcher = reinterpret_cast<FileManager*>(ctx);
			watcher->_impl->handleInotifyEvent(bev);
		}

	class StatException : public runtime_error {
		using runtime_error::runtime_error;
	};
};

void
RC2::FileManager::Impl::cleanup() {
	if (inotifyFd_ != -1)
		close(inotifyFd_);
	if (dbcon_)
		PQfinish(dbcon_);
}

void
RC2::FileManager::Impl::connect(string str, long wspaceId, long sessionRecId) 
{
	wspaceId_ = wspaceId;
	sessionRecId_ = sessionRecId;
	dbcon_ = PQconnectdb(str.c_str());
	//TODO: error checking
	char msg[255];
	sprintf(msg, "select projectid from workspace where id = %ld", wspaceId);
	projectId_ = query1long(msg);
	snprintf(msg, 255, "where wspaceid = %ld", wspaceId_);
	loadFiles(msg, false);
	if (projectId_ > 0) {
		fs::path spath = workingDir;
		spath /= "shared";
		fs::create_directory(spath);
		sprintf(msg, "where projectid = %ld", projectId_);
		loadFiles(msg, true);
	}
}

void
RC2::FileManager::Impl::loadFiles(const char *whereClause, bool isProject)
{
	ostringstream query;
	query << "select f.id::int4, f.version::int4, f.name, extract('epoch' from f.lastmodified)::int4, " 
		"d.bindata from rcfile f join rcfiledata d on f.id = d.id " << whereClause;
	DBResult res(PQexecParams(dbcon_, query.str().c_str(), 0, NULL, NULL, NULL, NULL, 1));
	ExecStatusType rc = PQresultStatus(res);
	if (PGRES_TUPLES_OK == rc) {
		int numfiles = PQntuples(res);
		for (int i=0; i < numfiles; i++) {
			uint32_t pid=0, pver=0, lastmod=0;
			string pname;
			char *ptr;
			ptr = PQgetvalue(res, i, 0);
			pid = ntohl(*(uint32_t*)ptr);
			ptr = PQgetvalue(res, i, 1);
			pver = ntohl(*(uint32_t*)ptr);
			pname = PQgetvalue(res, i, 2);
			ptr = PQgetvalue(res, i, 3);
			lastmod = ntohl(*(uint32_t*)ptr);
			int datalen = PQgetlength(res, i, 4);
			char *data = PQgetvalue(res, i, 4);
			DBFileInfoPtr filePtr(new DBFileInfo(pid, pver, pname, isProject));
			filesById_.insert(map<long,DBFileInfoPtr>::value_type(pid, filePtr));
			//write data to disk
			fs::path filepath(workingDir);
			if (isProject)
				filepath /= "shared";
			filepath /= pname;
			ofstream filest;
			filest.open(filepath.string(), ios::out | ios::trunc | ios::binary);
			filest.write(data, datalen);
			filest.close();
			//set modification time
			struct utimbuf modbuf;
			modbuf.actime = modbuf.modtime = lastmod;
			utime(filepath.c_str(), &modbuf);
		}
	} else {
		LOG(ERROR) << "sql error: " << PQresultErrorMessage(res) << endl;
	}
}

long 
RC2::FileManager::Impl::insertDBFile(string fname) {
	boost::smatch what;
	if (boost::regex_match(fname, what, imgRegex_, boost::match_default)) {
		cerr << "found image " << what[1] << endl;
		string filePath = workingDir + "/" + fname;
		ifstream file(filePath, ios::in|ios::binary|ios::ate);
		int size = file.tellg();
		unique_ptr<char[]> buffer(new char[size]);
		file.seekg(0, ios::beg);
		file.read(buffer.get(), size);
		file.close();
		long imgId = query1long("select nextval('sessionimage_seq'::regclass)");
		if (imgId <= 0)
			throw FormattedException("failed to get session image id");
		stringstream query;
		query << "insert into sessionimage (id,sessionid,name,imgdata) values (" << imgId 
			<< "," << sessionRecId_ << ",'img" << imgId << ".png',$1)";
		Oid in_oid[] = {1043,17};
		int pformats = 1;
		const char *params[] = {buffer.get()};
		DBResult res(PQexecParams(dbcon_, query.str().c_str(), 3, in_oid, params, 
			&size, &pformats, 1));
		ExecStatusType rc = PQresultStatus(res);
		if (!rc == PGRES_COMMAND_OK) {
			throw FormattedException("failed to insert image in db: %s", PQresultErrorMessage(res));
		}
		imageIds_.push_back(imgId);
		fs::remove(filePath);
		return 0;
	}
	cerr << "insert file:" << fname << endl;
	return 0;
}

void 
RC2::FileManager::Impl::updateDBFile(DBFileInfoPtr fobj) {
	cerr << "update file:" << fobj->name << endl;
	
}

void 
RC2::FileManager::Impl::removeDBFile(DBFileInfoPtr fobj) {
	cerr << "rm file:" << fobj->name << endl;
}

long
RC2::FileManager::Impl::query1long(const char *query)
{
	long value = 0;
	DBResult res(PQexec(dbcon_, query));
	ExecStatusType rc = PQresultStatus(res);
	if (PGRES_TUPLES_OK == rc) {
		value = atol(PQgetvalue(res, 0, 0));
	} else {
		LOG(ERROR) << "sql error:" << PQresultErrorMessage(res) << endl;
	}
	return value;
}

#pragma mark -

void
RC2::FileManager::Impl::setupInotify(FileManager *fm) {
	inotifyFd_ = inotify_init();
	if (inotifyFd_ == -1)
		throw FormattedException("inotify_init() failed for %s", workingDir.c_str());
	rootDir_.wd = inotify_add_watch(inotifyFd_, workingDir.c_str(), 
		IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVE);
	if (rootDir_.wd == -1)
		throw FormattedException("inotify_add_watched failed");
	stat(workingDir.c_str(), &rootDir_.sb);
	//TODO - watch shared directory
	for (auto itr=filesById_.begin(); itr != filesById_.end(); ++itr) {
		try {
			watchFile(itr->second);
		} catch (Impl::StatException &se) {
		}
	}
	
	struct bufferevent *evt = bufferevent_socket_new(eventBase_, inotifyFd_, 0);
	bufferevent_setcb(evt, FileManager::Impl::handleInotifyEvent, NULL, NULL, fm);
	bufferevent_enable(evt, EV_READ);
}

#define I_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void
RC2::FileManager::Impl::handleInotifyEvent(struct bufferevent *bev)
{
//	LOG(INFO) << "handleInotify called:" << workingDir << endl;
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		int evtype = event->mask & 0xffff; //events are in lower word, flags in upper
		LOG(INFO) << "notify:" << std::hex << evtype << " for " << 
			event->wd << endl;
		if(evtype == IN_CREATE) {
			long newFileId = insertDBFile((string)event->name);
			if (newFileId > 0) {
				//need to add a watch for this file
				watchFile(filesById_[newFileId]);
			}
		} else if (evtype == IN_CLOSE_WRITE) {
			updateDBFile(filesByWatchDesc_[event->wd]);
		} else if (evtype == IN_DELETE_SELF) {
			removeDBFile(filesByWatchDesc_[event->wd]);
			//discard our records of it
			inotify_rm_watch(inotifyFd_, event->wd);
		}
		//handle event
		p += sizeof(struct inotify_event) + event->len;
	}
}

void 
RC2::FileManager::Impl::watchFile(DBFileInfoPtr file) {
	string fullPath = workingDir + file->path;
	if (stat(fullPath.c_str(), &file->sb) == -1)
		throw StatException((format("stat failed for %s") % file->name).str());
	file->watchDescriptor = inotify_add_watch(inotifyFd_, fullPath.c_str(), 
				IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
	if (file->watchDescriptor == -1)
		throw FormattedException("inotify_add_warched failed %s", file->name.c_str());
	filesByWatchDesc_[file->watchDescriptor] = file;
	LOG(INFO) << "watching " << file->path << endl;
}


#pragma mark -


RC2::FileManager::FileManager()
	: _impl(new Impl())
{
	_impl->eventBase_ = nullptr;
	_impl->inotifyFd_ = -1;
}

RC2::FileManager::~FileManager()
{
	_impl->cleanup();
}

void
RC2::FileManager::initFileManager(string connectString, int wspaceId, int sessionRecId)
{
	_impl->connect(connectString, wspaceId, sessionRecId);
	_impl->setupInotify(this);
}

void
RC2::FileManager::setWorkingDir(std::string dir) 
{ 
	_impl->workingDir = dir; 
}

void
RC2::FileManager::setEventBase(struct event_base *eb)
{
	_impl->eventBase_ = eb;
}

void
RC2::FileManager::resetWatch()
{
	_impl->imageIds_.erase(_impl->imageIds_.begin(), _impl->imageIds_.end());
}

void
RC2::FileManager::checkWatch(vector<long> &imageIds)
{
	imageIds = _impl->imageIds_;
}

