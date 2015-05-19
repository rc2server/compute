#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include <postgresql/libpq-fe.h>

namespace RC2 {

	//for db fetched data
	struct DBFileInfo {
		long		id, version;
		std::string	name, path;
		int			watchDescriptor;
		struct stat	sb;
		bool		projectFile;
	
		DBFileInfo(uint32_t anId, uint32_t aVersion, std::string &aName, bool projFile=false)
			: id((long)anId), version((long)aVersion), name(aName), projectFile(projFile)
		{
			if (projectFile)
				path = "shared/";
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
	
			void	initializeSource(PGconn *con, long wsid, long projid);
			void	setWorkingDir(std::string workingDir);
			void	loadFiles(const char *whereClause, bool isProject);
			
			void	insertOrUpdateLocalFile(long fileId, long projId, long wspaceId);
			void	removeLocalFile(long fileId);
			
			long	insertDBFile(std::string fname, bool isProjectFile);
			void	updateDBFile(DBFileInfoPtr fobj);
			void	removeDBFile(DBFileInfoPtr fobj);
			
			void	loadRData();
			void	saveRData();
			
		std::map<long, DBFileInfoPtr>	filesById_;

		private:
			class Impl;
			std::unique_ptr<Impl>	_impl;
	};

};
