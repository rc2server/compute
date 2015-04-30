#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <set>
#include <iostream>
#include <iomanip>
#include <glog/logging.h>
#include <boost/format.hpp>
#include "InotifyFileWatcher.hpp"
#include "../common/FormattedException.hpp"

using namespace std;
using boost::format;

struct RC2::InotifyFileWatcher::FSObject 
{
	string			name;
	struct stat 	sb;
	int				wd;
	inline bool is_dir() const { return S_ISDIR(sb.st_mode); }
};

struct RC2::InotifyFileWatcher::Impl 
{
	int					inotifyFd;
	struct event_base*	eventBase;
	string				dirPath;
	map<int, FSObject>	files;
	FSObject			root;
	set<string>			addedFiles;
	set<string>			modifiedFiles;
	set<string>			deletedFiles;
	
	class StatException : public runtime_error {
		using runtime_error::runtime_error;
	};

	void watchFile(string fname) {
		FSObject fo;
		fo.name = fname;
		string fullPath = dirPath + "/" + fo.name;
		if (stat(fullPath.c_str(), &fo.sb) == -1)
			throw StatException((format("stat failed for %s") % fo.name).str());
		fo.wd = inotify_add_watch(inotifyFd, fullPath.c_str(), 
					IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
		if (fo.wd == -1)
			throw FormattedException("inotify_add_warched failed %s", fo.name.c_str());
		files[fo.wd] = fo;
		LOG(INFO) << "watching " << fo.name << endl;
	}
};


RC2::InotifyFileWatcher::InotifyFileWatcher()
	: _impl(new Impl())
{
	_impl->eventBase = nullptr;
	_impl->inotifyFd = -1;
}

RC2::InotifyFileWatcher::~InotifyFileWatcher()
{
	if (_impl->inotifyFd != -1)
		close(_impl->inotifyFd);
}

void
RC2::InotifyFileWatcher::setEventBase(struct event_base *eb)
{
	_impl->eventBase = eb;
}

void
RC2::InotifyFileWatcher::initializeWatcher(std::string dirPath)
{
	_impl->dirPath = dirPath;
	DIR *dir;
	class dirent *ent;

	_impl->inotifyFd = inotify_init();
	if (_impl->inotifyFd == -1)
		throw FormattedException("inotify_init() failed for %s", dirPath.c_str());
	_impl->root.wd = inotify_add_watch(_impl->inotifyFd, dirPath.c_str(), 
		IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVE);
	if (_impl->root.wd == -1)
		throw FormattedException("inotify_add_watched failed");
	stat(dirPath.c_str(), &_impl->root.sb);
	_impl->files[_impl->root.wd] = _impl->root;
	LOG(INFO) << "watching " << dirPath << endl;
	
	dir = opendir(dirPath.c_str());
	while ((ent = readdir(dir)) != NULL) {
		//skip "." and ".." but not other files that start with a period
		if (ent->d_name[0] == '.' && (strlen(ent->d_name) == 1 || ent->d_name[1] == '.'))
			continue;
		try {
			_impl->watchFile(ent->d_name);
		} catch (Impl::StatException &se) {
		}
	}
	closedir(dir);
	
	struct bufferevent *evt = bufferevent_socket_new(_impl->eventBase, _impl->inotifyFd, 0);
	bufferevent_setcb(evt, InotifyFileWatcher::handleInotifyEvent, NULL, NULL, static_cast<void*>(this));
	bufferevent_enable(evt, EV_READ);
}

#define I_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void
RC2::InotifyFileWatcher::handleInotifyEvent(struct bufferevent *bev)
{
	LOG(INFO) << "handleInotify called" << endl;
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		int evtype = event->mask & 0xffff; //events are in lower word, flags in upper
		LOG(INFO) << "notify:" << std::hex << evtype << " for " << 
			_impl->files[event->wd].name << endl;
		if(evtype == IN_CREATE) {
			_impl->addedFiles.insert((string)event->name);
			_impl->watchFile(event->name);
			//need to add a watch for this file
		} else if (evtype == IN_CLOSE_WRITE) {
			_impl->modifiedFiles.insert((string)event->name);
		} else if (evtype == IN_DELETE_SELF) {
			_impl->deletedFiles.insert((string)event->name);
			//discard our records of it
			inotify_rm_watch(_impl->inotifyFd, event->wd);
			_impl->files.erase(event->wd);
		}
		//handle event
		p += sizeof(struct inotify_event) + event->len;
	}
}

void
RC2::InotifyFileWatcher::startWatch()
{
	_impl->addedFiles.clear();
	_impl->modifiedFiles.clear();
	_impl->deletedFiles.clear();
}

void
RC2::InotifyFileWatcher::stopWatch(std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted)
{
	copy(_impl->addedFiles.begin(), _impl->addedFiles.end(), back_inserter(added));
	copy(_impl->modifiedFiles.begin(), _impl->modifiedFiles.end(), back_inserter(modified));
	copy(_impl->deletedFiles.begin(), _impl->deletedFiles.end(), back_inserter(deleted));
}

