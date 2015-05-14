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
#include <sys/types.h>
#include <utime.h>
#include "../common/FormattedException.hpp"

using namespace std;
using boost::format;
namespace fs = boost::filesystem;

#undef LOG
#define LOG(x) cerr

//for db fetched data
struct DBFileInfo {
	long	id, version;
	string	name;
	bool	projectFile;
	
	DBFileInfo(long anId, long aVersion, const char *aName, bool projFile=false) 
		: id(anId), version(aVersion), name(aName), projectFile(projFile)
	{}

	DBFileInfo(uint32_t anId, uint32_t aVersion, string &aName, bool projFile=false)
		: id((long)anId), version((long)aVersion), name(aName), projectFile(projFile)
	{}
	
	friend ostream& operator<<(ostream &out, DBFileInfo &finfo) {
		out << "(" << finfo.id << ": " << finfo.name << ")";
		return out;
	}
};

//for inotify
struct FSObject 
{
	string			name;
	struct stat 	sb;
	int				wd;
	inline bool is_dir() const { return S_ISDIR(sb.st_mode); }
};


class RC2::FileManager::Impl {
	public:
		long					wspaceId_;
		long					projectId_;
		PGconn*					dbcon_;
		map<long, DBFileInfo>	dbFiles_;
		map<int, FSObject>		fsFiles_;
		string					workingDir;
		struct event_base*		eventBase_;
		int						inotifyFd_;
		FSObject				root_;

		void cleanup(); //replacement for destructor
		void connect(string str, long wspaceId);
		void loadFiles(const char *whereClause, bool isProject);
		void insertDBFile(string fname);
		void updateDBFile(FSObject &fobj);
		void removeDBFile(FSObject &fobj);
		long query1long(const char *query);
		
		void	setupInotify(FileManager *fm);
		void 	watchFile(string fname);
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
RC2::FileManager::Impl::connect(string str, long wspaceId) 
{
	wspaceId_ = wspaceId;
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
	struct timespec start={0,0}, end={0,0};
	clock_gettime(CLOCK_MONOTONIC, &start);

	ostringstream query;
	query << "select f.id::int4, f.version::int4, f.name, extract('epoch' from f.lastmodified)::int4, " 
		"d.bindata from rcfile f join rcfiledata d on f.id = d.id " << whereClause;
	PGresult *res = PQexecParams(dbcon_, query.str().c_str(), 0, NULL, NULL, NULL, NULL, 1);
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
			dbFiles_.insert(map<long,DBFileInfo>::value_type(pid, DBFileInfo(pid, pver, pname, isProject)));
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
	PQclear(res);

	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("load files (%lu) took %.5f seconds\n", dbFiles_.size(),
		((double)end.tv_sec + 1.0e-9*end.tv_nsec) -
		((double)start.tv_sec + 1.0e-9*start.tv_nsec));
}

void 
RC2::FileManager::Impl::insertDBFile(string fname) {
	cerr << "insert file:" << fname << endl;
}

void 
RC2::FileManager::Impl::updateDBFile(FSObject &fobj) {
	cerr << "update file:" << fobj.name << endl;
	
}

void 
RC2::FileManager::Impl::removeDBFile(FSObject &fobj) {
	cerr << "rm file:" << fobj.name << endl;
}

long
RC2::FileManager::Impl::query1long(const char *query)
{
	long value = 0;
	PGresult *res = PQexec(dbcon_, query);
	ExecStatusType rc = PQresultStatus(res);
	if (PGRES_TUPLES_OK == rc) {
		value = atol(PQgetvalue(res, 0, 0));
	} else {
		LOG(ERROR) << "sql error:" << PQresultErrorMessage(res) << endl;
	}
	PQclear(res);
	return value;
}

#pragma mark -

void
RC2::FileManager::Impl::setupInotify(FileManager *fm) {
	DIR *dir;
	class dirent *ent;
	inotifyFd_ = inotify_init();
	if (inotifyFd_ == -1)
		throw FormattedException("inotify_init() failed for %s", workingDir.c_str());
	root_.wd = inotify_add_watch(inotifyFd_, workingDir.c_str(), 
		IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVE);
	if (root_.wd == -1)
		throw FormattedException("inotify_add_watched failed");
	stat(workingDir.c_str(), &root_.sb);
	fsFiles_[root_.wd] = root_;
	
	dir = opendir(workingDir.c_str());
	while ((ent = readdir(dir)) != NULL) {
		//skip "." and ".." but not other files that start with a period
		if (ent->d_name[0] == '.' && (strlen(ent->d_name) == 1 || ent->d_name[1] == '.'))
			continue;
		try {
			watchFile(ent->d_name);
		} catch (Impl::StatException &se) {
		}
	}
	closedir(dir);
	
	struct bufferevent *evt = bufferevent_socket_new(eventBase_, inotifyFd_, 0);
	bufferevent_setcb(evt, FileManager::Impl::handleInotifyEvent, NULL, NULL, fm);
	bufferevent_enable(evt, EV_READ);
}

#define I_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void
RC2::FileManager::Impl::handleInotifyEvent(struct bufferevent *bev)
{
	LOG(INFO) << "handleInotify called:" << workingDir << endl;
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		int evtype = event->mask & 0xffff; //events are in lower word, flags in upper
		LOG(INFO) << "notify:" << std::hex << evtype << " for " << 
			event->wd << endl;
		if(evtype == IN_CREATE) {
			insertDBFile((string)event->name);
			watchFile(event->name);
			//need to add a watch for this file
		} else if (evtype == IN_CLOSE_WRITE) {
			updateDBFile(fsFiles_[event->wd]);
		} else if (evtype == IN_DELETE_SELF) {
			removeDBFile(fsFiles_[event->wd]);
			//discard our records of it
			inotify_rm_watch(inotifyFd_, event->wd);
			fsFiles_.erase(event->wd);
		}
		//handle event
		p += sizeof(struct inotify_event) + event->len;
	}
}

void 
RC2::FileManager::Impl::watchFile(string fname) {
	FSObject fo;
	fo.name = fname;
	string fullPath = workingDir + "/" + fo.name;
	if (stat(fullPath.c_str(), &fo.sb) == -1)
		throw StatException((format("stat failed for %s") % fo.name).str());
	fo.wd = inotify_add_watch(inotifyFd_, fullPath.c_str(), 
				IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
	if (fo.wd == -1)
		throw FormattedException("inotify_add_warched failed %s", fo.name.c_str());
	fsFiles_[fo.wd] = fo;
	LOG(INFO) << "watching " << fo.name << endl;
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
RC2::FileManager::initFileManager(string connectString, int wspaceId)
{
	struct timespec start={0,0}, end={0,0};
	clock_gettime(CLOCK_MONOTONIC, &start);
	_impl->connect(connectString, wspaceId);
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("connection took %.5f seconds\n", 
		((double)end.tv_sec + 1.0e-9*end.tv_nsec) -
		((double)start.tv_sec + 1.0e-9*start.tv_nsec));
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
