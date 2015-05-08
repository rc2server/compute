#pragma once

#include <memory>
#include <event2/event.h>

namespace RC2 {

	class FileManager {
		public:
			FileManager();
			~FileManager();
			void 	initFileManager(std::string connectString, int wspaceId);
	
			void 	setWorkingDir(std::string dir);
			void	setEventBase(struct event_base *evbase);
	
		private:
			class Impl;
			std::unique_ptr<Impl>		_impl;
	};


};
