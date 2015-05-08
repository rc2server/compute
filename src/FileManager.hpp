#pragma once

#include <memory>

namespace RC2 {

	class FileManager {
		public:
			FileManager();
			~FileManager();
			void initFileManager(std::string connectString, int wspaceId);
	
			void setWorkingDir(std::string dir);
	
		private:
			class Impl;
			std::unique_ptr<Impl>		_impl;
	};


};
