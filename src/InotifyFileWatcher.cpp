#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <set>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include "InotifyFileWatcher.hpp"
#include "../common/FormattedException.hpp"

using namespace std;
namespace logging = boost::log;

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
};


RC2::InotifyFileWatcher::InotifyFileWatcher(struct event_base *eb)
	: _impl(new Impl())
{
	_impl->eventBase = eb;
	_impl->inotifyFd = -1;
}

RC2::InotifyFileWatcher::~InotifyFileWatcher()
{
	if (_impl->inotifyFd != -1)
		close(_impl->inotifyFd);
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
	BOOST_LOG_TRIVIAL(info) << "watching " << dirPath << endl;
	
	dir = opendir(dirPath.c_str());
	while ((ent = readdir(dir)) != NULL) {
		//skip "." and ".." but not other files that start with a period
		if (ent->d_name[0] == '.' && (strlen(ent->d_name) == 1 || ent->d_name[1] == '.'))
			continue;
		FSObject fo;
		fo.name = ent->d_name;
		string fullPath = dirPath + "/" + fo.name;
		if (stat(fullPath.c_str(), &fo.sb) == -1)
			continue;
		fo.wd = inotify_add_watch(_impl->inotifyFd, fullPath.c_str(), 
					IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
		if (fo.wd == -1)
			throw FormattedException("inotify_add_warched failed %s", fo.name.c_str());
		_impl->files[fo.wd] = fo;
		BOOST_LOG_TRIVIAL(info) << "watching " << fo.name << endl;
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
	BOOST_LOG_TRIVIAL(info) << "handleInotify called" << endl;
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		if(event->mask & IN_CREATE) {
			_impl->addedFiles.insert((string)event->name);
		} else if (event->mask & IN_CLOSE_WRITE) {
			_impl->modifiedFiles.insert((string)event->name);
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

