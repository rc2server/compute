#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include "../common/PGDBConnection.hpp"

namespace RC2 {

	//for db fetched data
	struct DBFileInfo {
		long		id, version;
		std::string	name, path;
		int			watchDescriptor;
		struct stat	sb;
	
		DBFileInfo(uint32_t anId, uint32_t aVersion, std::string &aName)
			: id((long)anId), version((long)aVersion), name(aName)
		{
			path += name;
		}
	
		friend std::ostream& operator<<(std::ostream &out, DBFileInfo &finfo) {
			out << "(" << finfo.id << ": " << finfo.name << ")";
			return out;
		}
	};


	typedef std::shared_ptr<DBFileInfo> DBFileInfoPtr;

	class DBFileSource {
		public:
				DBFileSource();
		virtual ~DBFileSource();
	
			void	initializeSource(std::shared_ptr<PGDBConnection> connection, long wsid);
			void	setWorkingDir(std::string workingDir);
			void	loadFiles(const char *whereClause);
			
			void	insertOrUpdateLocalFile(long fileId, long wspaceId);
			void	removeLocalFile(long fileId);
			
			long	insertDBFile(std::string fname);
			void	updateDBFile(DBFileInfoPtr fobj);
			void	removeDBFile(DBFileInfoPtr fobj);
			
			bool	loadRData();
			void	saveRData();
			
		std::map<long, DBFileInfoPtr>	filesById_;

		private:
			std::shared_ptr<PGDBConnection> dbcon_;
			class Impl;
			std::unique_ptr<Impl>	_impl;
	};

};
