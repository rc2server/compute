#pragma once
#include <string>
#include <vector>

namespace RC2 {

	class FileWatcher {
		
		public:
							FileWatcher();
			virtual			~FileWatcher();


			virtual void	initializeWatcher(std::string dirPath)=0;
			
			virtual void	checkFiles(
								std::vector<std::string> &added,  
								std::vector<std::string> &modified, 
								std::vector<std::string> &deleted
							)=0;
	
			FileWatcher(const FileWatcher&) = delete;
			FileWatcher& operator=(const FileWatcher&) = delete;
	};

};
