
#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <event2/event.h>
#include <boost/signals2.hpp>
#include "common/PGDBConnection.hpp"

namespace RC2 {
	enum ChangeType { UPDATE, INSERT, DELETE };
	
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
		FileInfo(const FileInfo &f2) 
		: id(f2.id), version(f2.version), name(f2.name)
		{ }
		FileInfo(const FileInfo* ptr) 
		: id(0), version(0), name("")
		{
			if (ptr) {
				id = ptr->id;
				version = ptr->version;
				name = ptr->name;
			}
		}
		FileInfo(int fileId, const FileInfo &other) 
		: id(fileId), version(other.version), name(other.name)
		{ }
		void operator=(FileInfo const &other) 
		{
			id = other.id;
			version = other.version;
			name = other.name;
		}
	};
	
	typedef FileInfo& FileInfoRef;
	typedef void(*FileListener)(long, ChangeType);
	class DBFileSource;
	
	class FileManager {
	public:
		FileManager();
		virtual ~FileManager();
		//the dbsrc parameter is for dependency injection
		virtual void 	initFileManager(std::string workingDir, std::shared_ptr<PGDBConnection> connection, 
										int wspaceId, int sessionRecId, 
										std::shared_ptr<DBFileSource> dbsrc =  std::shared_ptr<DBFileSource>());
		
		virtual std::string	getWorkingDir() const; //necessary for subclass to get variable stored in impl class
//		virtual void 	setWorkingDir(std::string dir);
		virtual void	setEventBase(struct event_base *evbase);
		
		virtual void	resetWatch();
		virtual void	checkWatch(std::vector<long> &imageIds, long &batchId);
		virtual void	cleanupImageWatch();
		
		virtual bool	loadRData();
		virtual void	saveRData();
		
		virtual bool	filePathForId(long fileId, std::string& filePath);
		virtual void	findOrAddFile(std::string fname, FileInfo &info);
		virtual bool	fileInfoForId(long fileId, FileInfo &info);
				void	allFiles(std::vector<std::shared_ptr<FileInfo>> &all);
		
		virtual void	suspendNotifyEvents();
		virtual void	resumeNotifyEvents();
		
		/**
		 * @brief Adds a listener function called when a file is changed. Caller is responsible to disconnect the connection
		 * 
		 * @param fileId id of file to listen for changes
		 * @param connection on return contains the connection object for the listener
		 * @param callback function called when a file changes
		 */
		virtual void	addChangeListener(long fileId, boost::signals2::connection& connection, FileListener callback);
		
		virtual void	setTitle(std::string title, std::string imageName);
		//for unit testing
		virtual void	processDBNotification(std::string message);

	private:
		class Impl;
		std::unique_ptr<Impl>		_impl;
	};
	
};
