#include <memory>
#include <map>
#include <string>
#include <sys/stat.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <boost/noncopyable.hpp>
#include "FileWatcher.hpp"

namespace RC2 {

class InotifyFileWatcher : public FileWatcher, private boost::noncopyable 
{
	public:
	
							InotifyFileWatcher(struct event_base *evbase);
		virtual				~InotifyFileWatcher();
		
			virtual void	initializeWatcher(std::string dirPath);
			
			virtual void	checkFiles(
								std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted
							);

	private:
		class FSObject {
			public:
			std::string		name;
			struct stat 	sb;
			int				wd;
			inline bool is_dir() const { return S_ISDIR(sb.st_mode); }
		};
	
		void	handleInotifyEvent(struct bufferevent *bev);
		static void handleInotifyEvent(struct bufferevent *bev, void *ctx)
		{
			InotifyFileWatcher *watcher = reinterpret_cast<InotifyFileWatcher*>(ctx);
			watcher->handleInotifyEvent(bev);
		}
	
		int									_inotifyFd;
		struct event_base*					_eventBase;
		std::string							_dirPath;
		std::map<int, FSObject>				_files;
		FSObject							_root;
};

};
