#include "FileManager.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <dirent.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <event2/bufferevent.h>
#include <postgresql/libpq-fe.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <sys/types.h>
#include <utime.h>
#include "../common/PostgresUtils.hpp"
#include "../common/FormattedException.hpp"
#include "common/RC2Utils.hpp"
#include "DBFileSource.hpp"
#include "RC2Logging.h"

using namespace std;
using boost::format;
namespace fs = boost::filesystem;

struct BoolChanger {
	bool *bptr, oldVal;
	BoolChanger(bool *var) : bptr(var) { oldVal = *bptr; *bptr = !oldVal; }
	~BoolChanger() { *bptr = oldVal; }
};


struct FSDirectory {
	int wd;
	struct stat sb;
};

class RC2::FileManager::Impl {
	public:
		long						wspaceId_;
		long						projectId_;
		long						sessionRecId_;
		DBFileSource				dbFileSource_;
		PGconn*						dbcon_;
		map<int, DBFileInfoPtr>		filesByWatchDesc_;
		vector<long>				imageIds_;
		string						workingDir;
		struct event_base*			eventBase_;
		int							inotifyFd_;
		FSDirectory					rootDir_;
		boost::regex				imgRegex_;
		bool						ignoreFSNotifications_;
		bool						ignoreDBNotifications_;

		Impl()
			: imgRegex_("rc2img(\\d+).png")
		{}

		void cleanup(); //replacement for destructor
		void connect(string str, long wspaceId, long sessionRecId);
		long insertImage(string fname, string imgNumStr);
		long query1long(const char *query);
		bool executeDBCommand(string cmd);
		
		unique_ptr<char[]> readFileBlob(DBFileInfoPtr fobj, size_t &size);

		void	setupInotify(FileManager *fm);
		void 	watchFile(DBFileInfoPtr file);
		void	handleInotifyEvent(struct bufferevent *bev);
		static void handleInotifyEvent(struct bufferevent *bev, void *ctx)
		{
			FileManager *watcher = reinterpret_cast<FileManager*>(ctx);
			watcher->_impl->handleInotifyEvent(bev);
		}

		void handleDBNotification();
		static void handleDBNotify(int fd, short event_type, void *ctx)
		{
			cerr << "handleDBNotify\n";
			FileManager *watcher = reinterpret_cast<FileManager*>(ctx);
			watcher->_impl->handleDBNotification();
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
	dbFileSource_.initializeSource(dbcon_, wspaceId_, projectId_);
	snprintf(msg, 255, "where wspaceid = %ld", wspaceId_);
	dbFileSource_.loadFiles(msg, false);
	if (projectId_ > 0) {
		fs::path spath = workingDir;
		spath /= "shared";
		fs::create_directory(spath);
		sprintf(msg, "where projectid = %ld", projectId_);
		dbFileSource_.loadFiles(msg, true);
	}
}

long
RC2::FileManager::Impl::insertImage(string fname, string imgNumStr)
{
	LOG(INFO) << "found image " << imgNumStr << endl;
	string filePath = workingDir + "/" + fname;
	size_t size;
	unique_ptr<char[]> buffer = ReadFileBlob(filePath, size);
	long imgId = query1long("select nextval('sessionimage_seq'::regclass)");
	if (imgId <= 0)
		throw FormattedException("failed to get session image id");
	stringstream query;
	query << "insert into sessionimage (id,sessionid,name,imgdata) values (" << imgId 
		<< "," << sessionRecId_ << ",'img" << imgId << ".png',$1)";
	Oid in_oid[] = {1043,17};
	int pformats = 1;
	int pSizes[] = {(int)size};
	const char *params[] = {buffer.get()};
	DBResult res(PQexecParams(dbcon_, query.str().c_str(), 1, in_oid, params, 
		pSizes, &pformats, 1));
	if (res.commandOK()) {
		throw FormattedException("failed to insert image in db: %s", res.errorMessage());
	}
	imageIds_.push_back(imgId);
	fs::remove(filePath);
	return 0;
}

void
RC2::FileManager::Impl::handleDBNotification()
{
	BoolChanger(&this->ignoreFSNotifications_);
	PGnotify *notify;
	PQconsumeInput(dbcon_);
	while((notify = PQnotifies(dbcon_)) != NULL) {
		if (!ignoreDBNotifications_) {
			char type = notify->extra[0];
			if (!(type == 'i' || type == 'u' || type == 'd') || strlen(notify->extra) < 2) {
				LOG(ERROR) << "bad db notification received:" << notify->extra << endl;
				continue;
			}
			long fileId = atol(&notify->extra[2]);
			cerr << "got notify for file " << fileId <<  "//  " << notify->extra << endl;
		}
		PQfreemem(notify);
	}
}

bool
RC2::FileManager::Impl::executeDBCommand(string cmd)
{
	DBResult res(PQexec(dbcon_, cmd.c_str()));
	if (res.commandOK()) {
		LOG(ERROR) << "sql error executing: " << cmd << " (" 
			<< res.errorMessage() << ")" << endl;
		return false;
	}
	return true;
}

long
RC2::FileManager::Impl::query1long(const char *query)
{
	long value = 0;
	DBResult res(PQexec(dbcon_, query));
	if (res.dataReturned()) {
		value = atol(PQgetvalue(res, 0, 0));
	} else {
		LOG(ERROR) << "sql error:" << res.errorMessage() << endl;
	}
	return value;
}

unique_ptr<char[]>
RC2::FileManager::Impl::readFileBlob(DBFileInfoPtr fobj, size_t &size)
{
	string filePath = workingDir + "/" + fobj->path;
	return ReadFileBlob(filePath, size);
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
	for (auto itr=dbFileSource_.filesById_.begin(); itr != dbFileSource_.filesById_.end(); ++itr) {
		try {
			watchFile(itr->second);
		} catch (Impl::StatException &se) {
		}
	}
	
	struct bufferevent *evt = bufferevent_socket_new(eventBase_, inotifyFd_, 0);
	bufferevent_setcb(evt, FileManager::Impl::handleInotifyEvent, NULL, NULL, fm);
	bufferevent_enable(evt, EV_READ);
	cerr << "setup notify handler\n";
}

#define I_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void
RC2::FileManager::Impl::handleInotifyEvent(struct bufferevent *bev)
{
	BoolChanger(&this->ignoreDBNotifications_);
	if (ignoreFSNotifications_)
		return;
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		int evtype = event->mask & 0xffff; //events are in lower word, flags in upper
		LOG(INFO) << "notify:" << std::hex << evtype << " for " << 
			event->wd << endl;
		if(evtype == IN_CREATE) {
			long newFileId=0;
			string fname = event->name;
			boost::smatch what;
			if (boost::regex_match(fname, what, imgRegex_, boost::match_default)) {
				newFileId = insertImage(fname, what[1]);
			} else {
				newFileId = dbFileSource_.insertDBFile(fname, event->wd != rootDir_.wd);
			}
			if (newFileId > 0) {
				//need to add a watch for this file
				watchFile(dbFileSource_.filesById_[newFileId]);
			}
		} else if (evtype == IN_CLOSE_WRITE) {
			dbFileSource_.updateDBFile(filesByWatchDesc_[event->wd]);
		} else if (evtype == IN_DELETE_SELF) {
			DBFileInfoPtr fobj = filesByWatchDesc_[event->wd];
			dbFileSource_.removeDBFile(fobj);
			filesByWatchDesc_.erase(fobj->id);
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
	DBResult listenRes(_impl->dbcon_, "listen rcfile");
	struct event *evt = event_new(_impl->eventBase_, PQsocket(_impl->dbcon_), 
		EV_READ|EV_PERSIST, RC2::FileManager::Impl::handleDBNotify, this);
	event_add(evt, NULL);
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

