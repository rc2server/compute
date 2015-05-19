#include <fstream>
#include <sstream>
#include <arpa/inet.h>
#include <utime.h>
#include "DBFileSource.hpp"
#include "../common/PostgresUtils.hpp"
#include "../common/FormattedException.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "common/RC2Utils.hpp"
#include "RC2Logging.h"

using namespace std;
using boost::format;
namespace fs = boost::filesystem;

class RC2::DBFileSource::Impl {
	public:
	PGconn *db_;
	long wspaceId_;
	long projId_;
	string workingDir_;
};


RC2::DBFileSource::DBFileSource()
{
}

RC2::DBFileSource::~DBFileSource()
{
}

void
RC2::DBFileSource::initializeSource(PGconn *con, long wsid, long projid)
{
	_impl->db_ = con;
	_impl->wspaceId_ = wsid;
	_impl->projId_ = projid;
}

void
RC2::DBFileSource::setWorkingDir(string workingDir)
{
	_impl->workingDir_ = workingDir;
}

void
RC2::DBFileSource::loadRData()
{
	string filepath = _impl->workingDir_ + "/.RData";
	ostringstream query;
	query << "select bindata from workspacedata where id = " << _impl->wspaceId_;
	DBResult res(PQexecParams(_impl->db_, query.str().c_str(), 0, NULL, NULL, NULL, NULL, 1));
	ExecStatusType rc = PQresultStatus(res);
	if (res.dataReturned()) {
		int datalen = PQgetlength(res, 0, 4);
		char *data = PQgetvalue(res, 0, 1);
		ofstream rdata;
		rdata.open(filepath, ios::out | ios::trunc | ios::binary);
		rdata.write(data, datalen);
		rdata.close();
	}
}

void
RC2::DBFileSource::saveRData()
{
	size_t newSize=0;
	string filePath = _impl->workingDir_ + "/.RData";
	if (!fs::exists(filePath))
		return;
	unique_ptr<char[]> data = ReadFileBlob(filePath, newSize);
	ostringstream query;
	query << "update workspacedata set bindata = $1 where id = " << _impl->wspaceId_;
	Oid in_oid[] = {1043};
	int pformats[] = {1};
	int pSizes[] = {(int)newSize};
	const char *params[] = {data.get()};
	DBResult res(PQexecParams(_impl->db_, query.str().c_str(), 1, in_oid, params, 
		pSizes, pformats, 1));
	if (!res.commandOK()) {
		throw FormattedException("failed to update workspacedata %ld: %s", _impl->wspaceId_, res.errorMessage());
	}
}

void
RC2::DBFileSource::loadFiles(const char *whereClause, bool isProject)
{
	ostringstream query;
	query << "select f.id::int4, f.version::int4, f.name, extract('epoch' from f.lastmodified)::int4, " 
		"d.bindata from rcfile f join rcfiledata d on f.id = d.id " << whereClause;
	DBResult res(PQexecParams(_impl->db_, query.str().c_str(), 0, NULL, NULL, NULL, NULL, 1));
	ExecStatusType rc = PQresultStatus(res);
	if (res.dataReturned()) {
		int numfiles = PQntuples(res);
		for (int i=0; i < numfiles; i++) {
			uint32_t pid=0, pver=0, lastmod=0;
			string pname;
			char *ptr;
			ptr = PQgetvalue(res, i, 0);
			pid = ntohl(*(uint32_t*)ptr);
			ptr = PQgetvalue(res, i, 1);
			pver = ntohl(*(uint32_t*)ptr);
			pname = PQgetvalue(res, i, 2);
			ptr = PQgetvalue(res, i, 3);
			lastmod = ntohl(*(uint32_t*)ptr);
			int datalen = PQgetlength(res, i, 4);
			char *data = PQgetvalue(res, i, 4);
			DBFileInfoPtr filePtr;
			if (filesById_.count(pid) > 0) {
				filePtr = filesById_.at(pid);
				filePtr->version = pver;
				filePtr->name = pname;
			} else {
				filePtr = DBFileInfoPtr(new DBFileInfo(pid, pver, pname, isProject));
				filesById_.insert(map<long,DBFileInfoPtr>::value_type(pid, filePtr));
			}
			//write data to disk
			fs::path filepath(_impl->workingDir_);
			if (isProject)
				filepath /= "shared";
			filepath /= pname;
			ofstream filest;
			filest.open(filepath.string(), ios::out | ios::trunc | ios::binary);
			filest.write(data, datalen);
			filest.close();
			//set modification time
			struct utimbuf modbuf;
			modbuf.actime = modbuf.modtime = lastmod;
			utime(filepath.c_str(), &modbuf);
		}
	} else {
		LOG(ERROR) << "sql error: " << res.errorMessage() << endl;
	}
}


void
RC2::DBFileSource::insertOrUpdateLocalFile(long fileId, long projId, long wspaceId)
{
	if (_impl->wspaceId_ != wspaceId && projId != _impl->projId_)
		return; //skip this file
	ostringstream where;
	where << "where f.id = " << fileId;
	loadFiles(where.str().c_str(), projId == _impl->projId_);
}

void
RC2::DBFileSource::removeLocalFile(long fileId)
{
	DBFileInfoPtr finfo = filesById_[fileId];
	fs::path fpath(_impl->workingDir_);
	fpath /= finfo->path;
	fs::remove(fpath);
	filesById_.erase(fileId);
//	filesByWatchDesc_.erase(fileId);
}

long
RC2::DBFileSource::insertDBFile(string fname, bool isProjectFile)
{
	string filePath = _impl->workingDir_ + "/" + (isProjectFile ? "shared/" : "") + fname;

	DBTransaction trans(_impl->db_);
	long fileId = DBLongFromQuery(_impl->db_, "select nextval('rcfile_seq'::regclass)");

	DBFileInfoPtr fobj(new DBFileInfo(fileId, 1, fname, isProjectFile));
	if (stat(filePath.c_str(), &fobj->sb) == -1)
		throw runtime_error((format("stat failed for %s") % fname).str());
	time_t modTime = fobj->sb.st_mtime;
	size_t newSize=0;
	unique_ptr<char[]> data = ReadFileBlob(filePath, newSize);

	char *escapedNamePtr = PQescapeLiteral(_impl->db_, fname.c_str(), fname.length());
	string escapedName = escapedNamePtr;
	PQfreemem(escapedNamePtr);
	string idname = isProjectFile ? "projectid" : "wspaceid";
	long typeId = isProjectFile ? _impl->projId_ : _impl->wspaceId_;
	ostringstream query;
	query << "insert into rcfile (id, version, " << idname 
		<< ",name,filesize,lastmodified) values = ("
		<< fileId << ", 1, " << typeId << ",'" << escapedName << "'," << newSize 
		<< "to_timestamp(" << modTime << "))";
	DBResult res1(_impl->db_, query.str());
	if (!res1.commandOK()) {
		throw FormattedException("failed to insert file %s: %s", fname.c_str(), res1.errorMessage());
	}
	query.clear();
	query.seekp(0);
	query << "insert into rcfiledata (id, bindata) values (" << fileId << ", $1)";
	Oid in_oid[] = {1043};
	int pformats[] = {1};
	int pSizes[] = {(int)newSize};
	const char *params[] = {data.get()};
	DBResult res2(PQexecParams(_impl->db_, query.str().c_str(), 1, in_oid, params, 
		pSizes, pformats, 1));
	if (!res2.commandOK()) {
		throw FormattedException("failed to insert file %s: %s", fname.c_str(), res2.errorMessage());
	}
	DBResult commitRes(trans.commit());
	if (!commitRes.commandOK()) {
		throw FormattedException("failed to commit file inserts %s: %s", fname.c_str(), 
			commitRes.errorMessage());
	}
	//need to insert fobj
	filesById_.insert(map<long,DBFileInfoPtr>::value_type(fileId, fobj));
	return fileId;
}

void 
RC2::DBFileSource::updateDBFile(DBFileInfoPtr fobj) 
{
	LOG(INFO) << "update file:" << fobj->name << endl;

	int newVersion = fobj->version + 1;
	string filePath = _impl->workingDir_ + "/" + fobj->path;
	if (stat(filePath.c_str(), &fobj->sb) == -1)
		throw runtime_error((format("stat failed for %s") % fobj->name).str());
	time_t newMod = fobj->sb.st_mtime;
	size_t newSize=0;
	unique_ptr<char[]> data = ReadFileBlob(filePath, newSize);

	DBTransaction trans(_impl->db_);
	ostringstream query;
	query << "update rcfile set version = " << newVersion << ", lastmodified = to_timestamp("
		<< newMod << "), filesize = " << newSize << " where id = " << fobj->id;
	DBResult res1(_impl->db_, query.str());
	if (!res1.commandOK()) {
		throw FormattedException("failed to update file %ld: %s", fobj->id, res1.errorMessage());
	}
	query.clear();
	query.seekp(0);
	query << "update rcfiledata set bindata = $1 where id = " << fobj->id;
	Oid in_oid[] = {1043};
	int pformats[] = {1};
	int pSizes[] = {(int)newSize};
	const char *params[] = {data.get()};
	DBResult res2(PQexecParams(_impl->db_, query.str().c_str(), 1, in_oid, params, 
		pSizes, pformats, 1));
	if (!res2.commandOK()) {
		throw FormattedException("failed to update file %ld: %s", fobj->id, res2.errorMessage());
	}
	DBResult commitRes(trans.commit());
	if (!commitRes.commandOK()) {
		throw FormattedException("failed to commit file updates %ld: %s", fobj->id, commitRes.errorMessage());
	}
	fobj->version = newVersion;
}

void 
RC2::DBFileSource::removeDBFile(DBFileInfoPtr fobj) 
{
	ostringstream query;
	query << "delete from rcfile where id = " << fobj->id;
	DBResult res(PQexec(_impl->db_, query.str().c_str()));
	if (res.commandOK()) {
		filesById_.erase(fobj->id);
	} else {
		LOG(ERROR) << "sql error delting file " << fobj->id << ":" 
			<< res.errorMessage() << endl;
	}
}
