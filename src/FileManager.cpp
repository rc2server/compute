#include "FileManager.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <postgresql/libpq-fe.h>
#include <glog/logging.h>
#include <libpqtypes.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <utime.h>

using namespace std;
namespace fs = boost::filesystem;


struct FileInfo {
	long	id, version;
	string	name;
	
	FileInfo(long anId, long aVersion, const char *aName) : id(anId), version(aVersion), 
		name(aName) 
	{}
	
	friend ostream& operator<<(ostream &out, FileInfo &finfo) {
		out << "(" << finfo.id << ": " << finfo.name << ")";
		return out;
	}
};

class RC2::FileManager::Impl {
	public:
		PGconn*				dbcon;
		vector<FileInfo>	wsFiles;
		vector<FileInfo>	pFiles;
		string				workingDir;

		void cleanup(); //replacement for destructor
		void connect(string str, long wspaceId);
		void loadFiles(string fileType, long typeId, string subfolder, vector<FileInfo> &files);
		long query1long(const char *query);
};

void
RC2::FileManager::Impl::cleanup() {
	if (dbcon)
		PQfinish(dbcon);
}

void
RC2::FileManager::Impl::connect(string str, long wspaceId) {
	dbcon = PQconnectdb(str.c_str());
	//TODO: error checking
	PQinitTypes(dbcon);
	char msg[255];
	loadFiles("wspaceid", wspaceId, "", wsFiles);
	sprintf(msg, "select projectid from workspace where id = %ld", wspaceId);
	long projId = query1long(msg);
	if (projId > 0) {
		fs::path spath = workingDir;
		spath /= "shared";
		fs::create_directory(spath);
		loadFiles("projectid", projId, "shared", pFiles);
	}
}

void
RC2::FileManager::Impl::loadFiles(string fileType, long typeId, string subfolder, 
	vector<FileInfo> &files)
{
	PGparam *param = PQparamCreate(dbcon);
	string query = "select f.id, f.version, f.name, f.datecreated, f.lastmodified, " 
		"d.bindata from rcfile f join rcfiledata d on f.id = d.id where f." + fileType + " = $1";
	cerr << "executing " << query << endl;
	if(!PQputf(param, "%int4", typeId))
		cerr << "prep error:" << PQgeterror() << endl;
	PGresult *res = PQparamExec(dbcon, param, query.c_str(), 1);
	ExecStatusType rc = PQresultStatus(res);
	if (PGRES_TUPLES_OK == rc) {
		int numfiles = PQntuples(res);
		for (int i=0; i < numfiles; i++) {
			PGint4 pid=0, pver=0;
			PGvarchar pname=NULL;
			PGtimestamp created, modified;
			PGbytea data;
			int success = PQgetf(res, i, "%int4 %int4 %varchar %timestamp %timestamp %bytea", 
				0, &pid, 1, &pver, 2, &pname, 3, &created, 4, &modified, 5, &data);
			if (!success)
				cerr << "error:" << PQgeterror() << endl;
			files.push_back(FileInfo(pid, pver, pname));
			fs::path filepath(workingDir);
			if (subfolder.length() > 0)
				filepath /= subfolder;
			filepath /= pname;
			ofstream filest;
			filest.open(filepath.string(), ios::out | ios::trunc | ios::binary);
			filest.write(data.data, data.len);
			filest.close();
			struct utimbuf modbuf;
			modbuf.actime = modbuf.modtime = modified.epoch;
			utime(filepath.c_str(), &modbuf);
		}
	} else {
		cerr << "sql error: " << PQresultErrorMessage(res) << endl;
	}
	PQclear(res);
}

long
RC2::FileManager::Impl::query1long(const char *query)
{
	long value = 0;
	PGresult *res = PQexec(dbcon, query);
	ExecStatusType rc = PQresultStatus(res);
	if (PGRES_TUPLES_OK == rc) {
		value = atol(PQgetvalue(res, 0, 0));
	} else {
		cerr << "sql error:" << PQresultErrorMessage(res) << endl;
	}
	PQclear(res);
	return value;
}

#pragma mark -


RC2::FileManager::FileManager()
	: _impl(new Impl())
{
}

RC2::FileManager::~FileManager()
{
	_impl->cleanup();
}

void
RC2::FileManager::initFileManager(string connectString, int wspaceId)
{
	_impl->connect(connectString, wspaceId);
}

void
RC2::FileManager::setWorkingDir(std::string dir) 
{ 
	_impl->workingDir = dir; 
}
