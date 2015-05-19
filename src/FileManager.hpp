#pragma once

#include <memory>
#include <vector>
#include <event2/event.h>

namespace RC2 {

	class FileManager {
		public:
			FileManager();
			~FileManager();
			void 	initFileManager(std::string connectString, int wspaceId, int sessionRecId);
	
			void 	setWorkingDir(std::string dir);
			void	setEventBase(struct event_base *evbase);
	
			void	resetWatch();
			void	checkWatch(std::vector<long> &imageIds);
			
			//for unit testing
			void	processDBNotification(std::string message);
		private:
			class Impl;
			std::unique_ptr<Impl>		_impl;
	};


};
