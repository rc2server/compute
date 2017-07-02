
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
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <utime.h>
#include "RC2Logging.h"
#include "../common/FormattedException.hpp"
#include "common/RC2Utils.hpp"
#include "common/ZeroInitializedStruct.hpp"
#include "DBFileSource.hpp"

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

struct PendingImage {
	string fileName;
	string imgNumStr;
	int wd;
	PendingImage(string fname, string iname, int desc)
		:fileName(fname), imgNumStr(iname), wd(desc)
		{}
};

using PendingImageMap = map<int,PendingImage>;

class RC2::FileManager::Impl : public ZeroInitializedClass {
	public:
		long						wspaceId_;
		long						sessionRecId_;
		long						sessionImageBatch_;
		shared_ptr<DBFileSource>	dbFileSource_;
		shared_ptr<PGDBConnection>	dbConnection_;
		set<string>					manuallyAddedFiles_;
		map<int, DBFileInfoPtr>		filesByWatchDesc_;
		PendingImageMap				pendingImagesByWatchDesc_;
		vector<long>				imageIds_;
		string						workingDir;
		struct event_base*			eventBase_;
		struct bufferevent*			inotifyEvent_;
		int							inotifyFd_;
		FSDirectory					rootDir_;
		boost::regex				imgRegex_;
		map<string, string>			imgNameToTitle_;
		bool						ignoreFSNotifications_;
		bool						ignoreDBNotifications_;

		Impl()
			: imgRegex_("rc2img(\\d+).png"), dbConnection_(new PGDBConnection())
		{}

		void cleanup(); //replacement for destructor
		void connect(std::shared_ptr<PGDBConnection> connection, long wspaceId, long sessionRecId);
		long insertImage(string fname, string imgNumStr);
		bool executeDBCommand(string cmd);
		
		unique_ptr<char[]> readFileBlob(DBFileInfoPtr fobj, size_t &size);
		bool fileExistsWithName(string fname);

		void	setupInotify(FileManager *fm);
		void 	watchFile(DBFileInfoPtr file);
		void	startImageWatch(string fname, string imgNum, inotify_event *event);
		void	stopImageWatch(int wd);
		void	cleanupImageWatch();
		void	handleInotifyEvent(struct bufferevent *bev);
		static void handleInotifyEvent(struct bufferevent *bev, void *ctx)
		{
			FileManager *watcher = reinterpret_cast<FileManager*>(ctx);
			watcher->_impl->handleInotifyEvent(bev);
		}

		void processDBNotification(string message);
		void handleDBNotifications();
		static void handleDBNotify(int fd, short event_type, void *ctx)
		{
			FileManager *watcher = reinterpret_cast<FileManager*>(ctx);
			watcher->_impl->handleDBNotifications();
		}
		
		static void handleIgnoreFSNotesOff(int fd, short event_type, void *ctx)
		{
			EventCallbackWrapper *wrapper = reinterpret_cast<EventCallbackWrapper*>(ctx);
			wrapper->impl->ignoreFSNotifications_ = false;
			event_del(wrapper->event);
			delete wrapper;
		}
		
		inline void fileInfoForDBPtr(DBFileInfoPtr ptr, FileInfo& info) 
		{
			info.id = ptr->id;
			info.version = ptr->version;
			info.name = ptr->name;
		}
		
		void ignoreFSNotifications();

	struct EventCallbackWrapper {
		struct event *event;
		RC2::FileManager::Impl *impl;
	};

	class StatException : public runtime_error {
		using runtime_error::runtime_error;
	};
};

void
RC2::FileManager::Impl::cleanup() {
	if (inotifyFd_ != -1)
		close(inotifyFd_);
}

void
RC2::FileManager::Impl::connect(std::shared_ptr<PGDBConnection> connection, long wspaceId, long sessionRecId) 
{
	wspaceId_ = wspaceId;
	sessionRecId_ = sessionRecId;
	dbConnection_ = connection;
	char msg[255];
	dbFileSource_->initializeSource(dbConnection_, wspaceId_);
	snprintf(msg, 255, "where wspaceid = %ld", wspaceId_);
	dbFileSource_->loadFiles(msg);
	sessionImageBatch_ = 0;
}

long
RC2::FileManager::Impl::insertImage(string fname, string imgNumStr)
{
	string title = "NULL";
	if (imgNameToTitle_.count(fname) > 0) {
		dbConnection_->escapeLiteral(imgNameToTitle_[fname], title);
	}
	string filePath = workingDir + "/" + fname;
	size_t size;
	unique_ptr<char[]> buffer = ReadFileBlob(filePath, size);
	if (size < 1) {
		LOG(INFO) << "got image with no data";
		return 0;
	}
	long imgId = dbConnection_->longFromQuery("select nextval('sessionimage_seq'::regclass)");
	if (imgId <= 0)
		throw FormattedException("failed to get session image id");
	if (sessionImageBatch_ <= 0) {
		stringstream batchq;
		batchq << "select max(batchid) from sessionimage where sessionid = " << sessionRecId_;
		sessionImageBatch_ = dbConnection_->longFromQuery(batchq.str().c_str()) + 1;
	}
	stringstream query;
	query << "insert into sessionimage (id,sessionid,batchid,name,title,imgdata) values (" << imgId 
		<< "," << sessionRecId_ << "," << sessionImageBatch_ 
		<< ",'img" << imgId << ".png',"
		<< title << ","
		<< "$1::bytea)";
	int pformats = 1;
	int pSizes[] = {(int)size};
 	const char *params[] = {buffer.get()};
	DBResult res = dbConnection_->executeQuery(query.str(), 1, NULL, params, pSizes, &pformats);
	if (!res.commandOK()) {
		LOG(WARNING) << "insert image error:" << res.errorMessage();
		throw FormattedException("failed to insert image in db: %s", res.errorMessage());
	}
//	LOG(INFO) << "inserted image " << imgId << " of size " << size;
	imageIds_.push_back(imgId);
	ignoreFSNotifications();
    if (NULL == getenv("RSESSION_KEEP_IMAGES"))
        fs::remove(filePath);
	return 0;
}

void RC2::FileManager::Impl::startImageWatch ( string fname, string imgNum, inotify_event* event )
{
	string fullPath = workingDir + "/" + fname;
	int wd = inotify_add_watch(inotifyFd_, fullPath.c_str(), IN_CLOSE_WRITE | IN_CLOSE_NOWRITE);
	if (wd == -1)
		throw FormattedException("inotify_add_warched failed %s", fname.c_str());
	pendingImagesByWatchDesc_.insert(std::make_pair(wd, PendingImage(fname, imgNum, wd)));
}

void RC2::FileManager::Impl::stopImageWatch ( int wd )
{
	inotify_rm_watch(inotifyFd_, wd);
	pendingImagesByWatchDesc_.erase(wd);
}

void RC2::FileManager::Impl::cleanupImageWatch()
{
	for(auto it = pendingImagesByWatchDesc_.begin(); it != pendingImagesByWatchDesc_.end(); ++it) {
//		LOG(INFO) << "cleaning up image " << it->second.fileName;
		//check to see if image exists on disk with file size > 0
		fs::path imgPath(workingDir);
		imgPath /= it->second.fileName;
		auto size = fs::file_size(imgPath);
		if (size !=  static_cast<uintmax_t>(-1)) {
			insertImage(it->second.fileName, it->second.imgNumStr);
			//have to manually stopImageWatch so itr isn't mutated while looping
			inotify_rm_watch(inotifyFd_, it->second.wd);
		}
	}
	//clear set
	pendingImagesByWatchDesc_.erase(pendingImagesByWatchDesc_.begin(), pendingImagesByWatchDesc_.end());
}


void
RC2::FileManager::Impl::processDBNotification(string message)
{
	LOG(INFO) << "db notification: " << message;
	char type = message.c_str()[0];
	if (!(type == 'i' || type == 'u' || type == 'd') || message.length() < 2) {
		LOG(WARNING) << "bad db notification received:" << message;
		return;
	}
	//figure out fileId and wspaceId from message
	string numStr(message.substr(1));
	vector<string> fields;
	boost::split(fields, numStr, boost::is_any_of("/"));
	if (fields.size() != 3) {
		LOG(WARNING) << "bad db notification received:" << message;
		return;
	}
	long fileId = atol(fields[0].c_str());
	long fileWspaceId = atol(fields[1].c_str());
	if (wspaceId_ != fileWspaceId)
		return;
	try {
		if (type == 'd') {
			try {
				DBFileInfoPtr fobj = dbFileSource_->filesById_.at(fileId);
				//stop notify watch first
				inotify_rm_watch(inotifyFd_, fobj->watchDescriptor);
				dbFileSource_->filesById_.erase(fileId);
				filesByWatchDesc_.erase(fileId);
				fs::remove(workingDir + "/" + fobj->path);
			} catch (out_of_range &ore) {
				//we'll just ignore
				LOG(WARNING) << "got db note to delete " << fileId << 
					" but we dont' have a desc for it:" << ore.what();
			} catch (exception &ee) {
				//we'll just ignore
				LOG(WARNING) << "XX got db note to delete " << fileId << 
					" but we dont' have a desc for it:" << ee.what();
			}
		} else {
			ostringstream query;
			query << " where f.id = " << fileId;
			if (type == 'i') {
				LOG(INFO) << "got insert for " << fileId;
				ignoreFSNotifications();
				dbFileSource_->loadFiles(query.str().c_str());
				watchFile(dbFileSource_->filesById_[fileId]);
			} else if (type == 'u') {
				LOG(INFO) << "got update for " << fileId;
				if (dbFileSource_->filesById_.count(fileId) > 0) {
					ignoreFSNotifications();
					dbFileSource_->loadFiles(query.str().c_str());
				}
			}
		}
	} catch (exception &e) {
		LOG(WARNING) << "exception handling db notification: " << e.what();
	}
}

void
RC2::FileManager::Impl::handleDBNotifications()
{
	LOG(INFO) << "handleDBNotifications called";
	ignoreFSNotifications();
	PGnotify *notify;
	dbConnection_->consumeInput();
	string name, channel;
	while (dbConnection_->checkForNotification(name, channel)) {
		LOG(INFO) << "got notification " << channel;
		if (!ignoreDBNotifications_)
			processDBNotification(channel);
	}
}

bool
RC2::FileManager::Impl::executeDBCommand(string cmd)
{
	DBResult res = dbConnection_->executeQuery(cmd);
	if (res.commandOK()) {
		LOG(WARNING) << "sql error executing: " << cmd << " (" 
			<< res.errorMessage() << ")";
		return false;
	}
	return true;
}

unique_ptr<char[]>
RC2::FileManager::Impl::readFileBlob(DBFileInfoPtr fobj, size_t &size)
{
	string filePath = workingDir + "/" + fobj->path;
	return ReadFileBlob(filePath, size);
}

//this should be called to turn ignoreFSNotes on. It schedules a low priority event
// to turn it off. That way any higher priority events can be processed before turing this flag off
void
RC2::FileManager::Impl::ignoreFSNotifications() 
{
	if (ignoreFSNotifications_) { return; } //already set
	ignoreFSNotifications_ = true;
	struct timeval timeout = {0,5000}; //5 milliseconds
	struct EventCallbackWrapper *wrapper = new EventCallbackWrapper();
	wrapper->impl = this;
	struct event *evt = event_new(eventBase_, -1, 
		EV_TIMEOUT, RC2::FileManager::Impl::handleIgnoreFSNotesOff, wrapper);
	wrapper->event = evt;
	event_priority_set(evt, 2); //so inotify events handled first
	event_add(evt, &timeout);
}

bool
RC2::FileManager::Impl::fileExistsWithName(string fname) {
	auto & fileMap = dbFileSource_->filesById_;
	for (auto itr = fileMap.begin(); itr != fileMap.end(); ++itr) {
		DBFileInfoPtr ptr = itr->second;
		if (0 == fname.compare(ptr->name)) {
			return true;
		}
	}
	return false;
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
	for (auto itr=dbFileSource_->filesById_.begin(); itr != dbFileSource_->filesById_.end(); ++itr) {
		try {
			watchFile(itr->second);
		} catch (Impl::StatException &se) {
		}
	}
	
	struct bufferevent *evt = bufferevent_socket_new(eventBase_, inotifyFd_, 0);
	bufferevent_priority_set(evt, 0); //high priority
	bufferevent_setcb(evt, FileManager::Impl::handleInotifyEvent, NULL, NULL, fm);
	bufferevent_enable(evt, EV_READ);
	inotifyEvent_ = evt;
}

#define I_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void
RC2::FileManager::Impl::handleInotifyEvent(struct bufferevent *bev)
{
	bool ignoring = ignoreFSNotifications_;
	if (!ignoring)
		ignoreFSNotifications();
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	if (ignoring) {
		LOG(INFO) << "ignoring " << numRead << "inotify events";
		return;
	}
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		int evtype = event->mask & 0xffff; //events are in lower word, flags in upper
		LOG(INFO) << "notify:" << std::hex << event->mask;
		try {
			if(evtype == IN_CREATE) {
				if (!(event->mask & IN_ISDIR)) { //we don't want these events, they are duplicates
					long newFileId=0;
					string fname = event->name;
					boost::smatch what;
					if (event->name[0] != '.') {
						if (boost::regex_match(fname, what, imgRegex_, boost::match_default)) {
							startImageWatch(fname, what[1], event);
//							newFileId = insertImage(fname, what[1]);
						} else if (manuallyAddedFiles_.find(fname) == manuallyAddedFiles_.end()) {
							if (fileExistsWithName(fname)) {
								LOG(INFO) << "create for existing file " << fname;
							} else {
								LOG(INFO) << "inotify create for " << fname << ": " << manuallyAddedFiles_.size();
								newFileId = dbFileSource_->insertDBFile(fname);
								LOG(INFO) << "inserted s " << newFileId;
							}
						}
						if (newFileId > 0) {
							//need to add a watch for this file
							watchFile(dbFileSource_->filesById_[newFileId]);
						}
					}
				}
			} else if (evtype == IN_CLOSE_WRITE) {
				auto imgItr = pendingImagesByWatchDesc_.find(event->wd);
				if (imgItr != pendingImagesByWatchDesc_.end()) {
					insertImage(imgItr->second.fileName, imgItr->second.imgNumStr);
					stopImageWatch(event->wd);
				} else {
					DBFileInfoPtr fobj = filesByWatchDesc_[event->wd];
					LOG(INFO) << "got close write event for " << fobj->name;
					dbFileSource_->updateDBFile(fobj);
				}
			} else if (evtype == IN_DELETE_SELF) {

				DBFileInfoPtr fobj = filesByWatchDesc_[event->wd];
			LOG(INFO) << "got delete event for " << fobj->name;
				dbFileSource_->removeDBFile(fobj);
				filesByWatchDesc_.erase(fobj->id);
				//discard our records of it
				inotify_rm_watch(inotifyFd_, event->wd);
			} else if (evtype == IN_OPEN) {
				DBFileInfoPtr fobj = filesByWatchDesc_[event->wd];
				LOG(INFO) << "got open for " << fobj->name;
			} else if (evtype == IN_MODIFY) {
				DBFileInfoPtr fobj = filesByWatchDesc_[event->wd];
				LOG(INFO) << "got write for " << fobj->name;
			}
		} catch(exception &ex) {
			LOG(WARNING) << "exception in inotify code: " << ex.what();
		}
		//handle event
		p += sizeof(struct inotify_event) + event->len;
	}
}

void 
RC2::FileManager::Impl::watchFile(DBFileInfoPtr file) {
	string fullPath = workingDir + "/" + file->path;
	if (stat(fullPath.c_str(), &file->sb) == -1)
		throw StatException((format("stat failed for watch %s") % fullPath.c_str()).str());
	file->watchDescriptor = inotify_add_watch(inotifyFd_, fullPath.c_str(), 
											  IN_OPEN | IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY | IN_EXCL_UNLINK);
	if (file->watchDescriptor == -1)
		throw FormattedException("inotify_add_warched failed %s", file->name.c_str());
	filesByWatchDesc_[file->watchDescriptor] = file;
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
RC2::FileManager::initFileManager(std::string workingDir, std::shared_ptr<PGDBConnection> connection, int wspaceId, 
								  int sessionRecId, std::shared_ptr<DBFileSource> dbsrc)
{
	_impl->dbFileSource_ = dbsrc;
	if (!_impl->dbFileSource_)
		_impl->dbFileSource_ = make_shared<DBFileSource>();
	_impl->workingDir = workingDir;
	_impl->dbFileSource_->setWorkingDir(workingDir);
	_impl->connect(connection, wspaceId, sessionRecId);
	_impl->setupInotify(this);
	DBResult listenRes = _impl->dbConnection_->executeQuery("listen rcfile");
	struct event *evt = event_new(_impl->eventBase_,_impl->dbConnection_->getSocket(), 
		EV_READ|EV_PERSIST, RC2::FileManager::Impl::handleDBNotify, this);
	event_priority_set(evt, 2); //so inotify events handled first
	event_add(evt, NULL);
}

void RC2::FileManager::suspendNotifyEvents()
{
	if (bufferevent_get_enabled(_impl->inotifyEvent_) != 0)
		throw std::runtime_error("notify events already suspended");
	bufferevent_disable(_impl->inotifyEvent_, EV_READ);
}

void RC2::FileManager::resumeNotifyEvents()
{
	if (bufferevent_get_enabled(_impl->inotifyEvent_) == 0)
		throw std::runtime_error("notify events already enabled");
	bufferevent_disable(_impl->inotifyEvent_, EV_READ);
}

string RC2::FileManager::getWorkingDir() const
{
	return _impl->workingDir;
}

// void
// RC2::FileManager::setWorkingDir(std::string dir) 
// { 
// 	_impl->workingDir = dir; 
// //	_impl->dbFileSource_->setWorkingDir(dir);
// }

void
RC2::FileManager::setEventBase(struct event_base *eb)
{
	_impl->eventBase_ = eb;
}

void
RC2::FileManager::resetWatch()
{
//	LOG(INFO) << "fm:resetWatch called: " << _impl->imageIds_.size();
	if (_impl->imageIds_.size() > 0) {
		_impl->sessionImageBatch_++;
	}
	_impl->imageIds_.erase(_impl->imageIds_.begin(), _impl->imageIds_.end());
	_impl->manuallyAddedFiles_.clear();
}

void
RC2::FileManager::checkWatch(vector<long> &imageIds, long &batchId)
{
	imageIds = _impl->imageIds_;
	batchId = _impl->sessionImageBatch_;
//	_impl->sessionImageBatch_ = 0;
}

void RC2::FileManager::setTitle(std::string title, std::string imageName)
{
	_impl->imgNameToTitle_[imageName] = title;
}

void RC2::FileManager::cleanupImageWatch()
{
	_impl->cleanupImageWatch();
	_impl->imgNameToTitle_.clear();
}

bool
RC2::FileManager::loadRData()
{
	return _impl->dbFileSource_->loadRData();
}

void
RC2::FileManager::saveRData()
{
	_impl->dbFileSource_->saveRData();
}

void
RC2::FileManager::findOrAddFile(std::string fname, FileInfo &info)
{
	auto & fileMap = _impl->dbFileSource_->filesById_;
	for (auto itr = fileMap.begin(); itr != fileMap.end(); ++itr) {
		DBFileInfoPtr ptr = itr->second;
		if (0 == fname.compare(ptr->name)) {
			//a match. 
			_impl->fileInfoForDBPtr(ptr, info);
			return;
		}
	}
	//need to add the file
	long fid = _impl->dbFileSource_->insertDBFile(fname);
	_impl->manuallyAddedFiles_.insert(fname);
	_impl->fileInfoForDBPtr(_impl->dbFileSource_->filesById_[fid], info);
}

bool 
RC2::FileManager::fileInfoForId (long fileId, FileInfo& info)
{
	auto & fileCache = _impl->dbFileSource_->filesById_;
	if (fileCache.count(fileId) != 1)
		return false;
	auto dbptr = fileCache[fileId];
	info.id = dbptr->id;
	info.version = dbptr->version;
	info.name = dbptr->name;
	return true;
}


bool
RC2::FileManager::filePathForId(long fileId, string& filePath)
{
	auto & fileCache = _impl->dbFileSource_->filesById_;
	if (fileCache.count(fileId) != 1) {
		LOG(WARNING) << "filenameForId called with invalid id: " << fileId;
		return false;
	}
	filePath = fileCache[fileId]->path;
	return true;
}

void
RC2::FileManager::processDBNotification(string message)
{
	_impl->processDBNotification(message);
}
