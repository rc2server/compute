#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include "InotifyFileWatcher.hpp"
#include "../common/FormattedException.hpp"

using namespace std;

struct RC2::InotifyFileWatcher::FSObject 
{
	std::string		name;
	struct stat 	sb;
	int				wd;
	inline bool is_dir() const { return S_ISDIR(sb.st_mode); }
};

struct RC2::InotifyFileWatcher::Impl 
{
	int							inotifyFd;
	struct event_base*			eventBase;
	std::string					dirPath;
	std::map<int, FSObject>		files;
	FSObject					root;
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
	stat(dirPath.c_str(), &_impl->root.sb);
	_impl->files[_impl->root.wd] = _impl->root;
	
	dir = opendir(dirPath.c_str());
	while ((ent = readdir(dir)) != NULL) {
		FSObject fo;
		fo.name = ent->d_name;
		string fullPath = dirPath + "/" + fo.name;
		if (stat(fullPath.c_str(), &fo.sb) == -1)
			continue;
		fo.wd = inotify_add_watch(_impl->inotifyFd, fullPath.c_str(), 
					IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
		_impl->files[fo.wd] = fo;
	}
	closedir(dir);
	
	struct bufferevent *evt = bufferevent_socket_new(_impl->eventBase, _impl->inotifyFd, 0);
	bufferevent_setcb(evt, InotifyFileWatcher::handleInotifyEvent, NULL, NULL, static_cast<void*>(this));
	bufferevent_enable(evt, EV_READ);
}

void
RC2::InotifyFileWatcher::handleInotifyEvent(struct bufferevent *event)
{
}

void
RC2::InotifyFileWatcher::startWatch()
{
}

void
RC2::InotifyFileWatcher::stopWatch(std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted)
{

}

