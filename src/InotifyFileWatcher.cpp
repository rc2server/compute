#include <dirent.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include "InotifyFileWatcher.hpp"
#include "../common/FormattedException.hpp"

using namespace std;


RC2::InotifyFileWatcher::InotifyFileWatcher(struct event_base *eb)
	: _eventBase(eb), _inotifyFd(-1)
{
}

RC2::InotifyFileWatcher::~InotifyFileWatcher()
{
	if (_inotifyFd != -1)
		close(_inotifyFd);
}

void
RC2::InotifyFileWatcher::initializeWatcher(std::string dirPath)
{
	_dirPath = dirPath;
	DIR *dir;
	class dirent *ent;

	_inotifyFd = inotify_init();
	if (_inotifyFd == -1)
		throw FormattedException("inotify_init() failed for %s", dirPath.c_str());
	_root.wd = inotify_add_watch(_inotifyFd, dirPath.c_str(), 
		IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVE);
	stat(dirPath.c_str(), &_root.sb);
	_files[_root.wd] = _root;
	
	dir = opendir(dirPath.c_str());
	while ((ent = readdir(dir)) != NULL) {
		FSObject fo;
		fo.name = ent->d_name;
		string fullPath = dirPath + "/" + fo.name;
		if (stat(fullPath.c_str(), &fo.sb) == -1)
			continue;
		fo.wd = inotify_add_watch(_inotifyFd, fullPath.c_str(), 
					IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
		_files[fo.wd] = fo;
	}
	closedir(dir);
	
	struct bufferevent *evt = bufferevent_socket_new(_eventBase, _inotifyFd, 0);
	bufferevent_setcb(evt, InotifyFileWatcher::handleInotifyEvent, NULL, NULL, static_cast<void*>(this));
	bufferevent_enable(evt, EV_READ);
}

void
RC2::InotifyFileWatcher::handleInotifyEvent(struct bufferevent *event)
{
}
			
void
RC2::InotifyFileWatcher::checkFiles(std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted)
{

}

