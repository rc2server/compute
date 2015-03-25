#include <memory>
#include <map>
#include <string>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <boost/noncopyable.hpp>
#include "FileWatcher.hpp"

namespace RC2 {

class InotifyFileWatcher : public FileWatcher, private boost::noncopyable 
{
	public:
	
							InotifyFileWatcher();
		virtual				~InotifyFileWatcher();
		
					void	setEventBase(struct event_base *evbase);
			virtual void	initializeWatcher(std::string dirPath);
			
			virtual void	startWatch();
			
			virtual void	stopWatch(
								std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted
							);

	private:
	
		void	handleInotifyEvent(struct bufferevent *bev);
		static void handleInotifyEvent(struct bufferevent *bev, void *ctx)
		{
			InotifyFileWatcher *watcher = reinterpret_cast<InotifyFileWatcher*>(ctx);
			watcher->handleInotifyEvent(bev);
		}
	
		class FSObject;
		struct Impl;
		std::unique_ptr<Impl>	_impl;
};

};
