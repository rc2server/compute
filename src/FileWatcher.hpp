#pragma once
#include <string>
#include <vector>

namespace RC2 {

	class FileWatcher {
		
		public:
							FileWatcher()=default;
			virtual			~FileWatcher()=default;


			virtual void	initializeWatcher(std::string dirPath)=0;
			
			virtual void	startWatch()=0;
			
			virtual void	stopWatch(
								std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted
							)=0;
	
			FileWatcher(const FileWatcher&) = delete;
			FileWatcher& operator=(const FileWatcher&) = delete;
	};

};
