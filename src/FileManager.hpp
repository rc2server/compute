#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <event2/event.h>

namespace RC2 {

	struct FileInfo {
		long id;
		long version;
		std::string name;
		FileInfo()
			: id(0), version(0), name("")
			{}
		FileInfo(long anId, long aVersion, std::string aName)
			: id(anId), version(aVersion), name(aName)
			{}
		FileInfo(const FileInfo* ptr) 
			: id(0), version(0), name("")
		{
			if (ptr) {
				id = ptr->id;
				version = ptr->version;
				name = ptr->name;
			}
		}
	};
	
	class FileManager {
		public:
			FileManager();
			~FileManager();
			void 	initFileManager(std::string connectString, int wspaceId, int sessionRecId);
	
			void 	setWorkingDir(std::string dir);
			void	setEventBase(struct event_base *evbase);
	
			void	resetWatch();
			void	checkWatch(std::vector<long> &imageIds, long &batchId);
			void	cleanupImageWatch();
			
			bool	loadRData();
			void	saveRData();
			
			bool	filePathForId(long fileId, std::string& filePath);
			void	findOrAddFile(std::string fname, FileInfo &info);
			bool	fileInfoForId(long fileId, FileInfo &info);
			
			void	suspendNotifyEvents();
			void	resumeNotifyEvents();
			
			//for unit testing
			void	processDBNotification(std::string message);
		private:
			class Impl;
			std::unique_ptr<Impl>		_impl;
	};

};
