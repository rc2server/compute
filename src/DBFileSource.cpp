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
	long wspaceId_;
	string workingDir_;
};


RC2::DBFileSource::DBFileSource()
	: _impl(new Impl())
{
}

RC2::DBFileSource::~DBFileSource()
{
}

void
RC2::DBFileSource::initializeSource(std::shared_ptr<PGDBConnection> connection, long wsid)
{
	dbcon_ = connection;
	_impl->wspaceId_ = wsid;
	//verify workspace exists in database
	ostringstream query;
	query << "select id,name from rcworkspace where id = " << wsid;
	DBResult res = connection->executeQuery(query.str());
	if (res.rowsReturned() < 1)
		throw std::runtime_error((format("invalid workspace id: %1%") % wsid).str());
}

void
RC2::DBFileSource::setWorkingDir(string workingDir)
{
	_impl->workingDir_ = workingDir;
}

bool
RC2::DBFileSource::loadRData()
{
	string filepath = _impl->workingDir_ + "/.RData";
	ostringstream query;
	query << "select bindata from rcworkspacedata where id = " << _impl->wspaceId_;
	DBResult res = dbcon_->executeQuery(query.str(), 0, NULL, NULL, NULL, NULL, 1);
//	ExecStatusType rc = PQresultStatus(res);
	if (res.dataReturned()) {
		int datalen = res.getLength(0, 0);
		if (datalen > 0) {
			char *data = res.getValue(0, 0);
			ofstream rdata;
			rdata.open(filepath, ios::out | ios::trunc | ios::binary);
			rdata.write(data, datalen);
			rdata.close();
			return true;
		}
	}
	LOG(INFO) << ".RData not loaded" << std::endl;
	return false;
}

void
RC2::DBFileSource::saveRData()
{
	size_t newSize=0;
	string filePath = _impl->workingDir_ + "/.RData";
	if (!fs::exists(filePath)) {
		LOG(INFO) << "no .RData file to save" << std::endl;
		return;
	}
	unique_ptr<char[]> data = ReadFileBlob(filePath, newSize);
	DBTransaction trans = dbcon_->startTransaction();
	DBResult lockRes = dbcon_->executeQuery("lock table rcworkspacedata in access exclusive mode");
	if (!lockRes.commandOK()) {
		LOG(WARNING) << "saveRData failed to get lock on table" << std::endl;
		return;
	}
	ostringstream query;
	query << "update rcworkspacedata set bindata = $1::bytea where id = " << _impl->wspaceId_;
	int pformats[] = {1};
	int pSizes[] = {(int)newSize};
	const char *params[] = {data.get()};
	DBResult res = dbcon_->executeQuery(query.str(), 1, NULL, params, pSizes, pformats, 1);
	if (!res.commandOK()) {
		throw FormattedException("failed to update rcworkspacedata %ld: %s", _impl->wspaceId_, res.errorMessage());
	}
	if (res.rowsAffected() < 1) {
		ostringstream iquery;
		iquery << "insert into rcworkspacedata (id,bindata) values (" << _impl->wspaceId_ << ", $1::bytea)";
		DBResult iRes = dbcon_->executeQuery(iquery.str(), 1, NULL, params, pSizes, pformats, 1);
		if (!iRes.commandOK()) {
			throw FormattedException("failed to insert rcworkspacedata %ld:%s", _impl->wspaceId_, iRes.errorMessage());
		}
	}
	DBResult commitRes(trans.commit());
	if (!commitRes.commandOK()) {
		throw FormattedException("failed to commit save rdata");
	}
}

void
RC2::DBFileSource::loadFiles(const char *whereClause)
{
	ostringstream query;
	query << "select f.id::int4, f.version::int4, f.name, extract('epoch' from f.lastmodified)::int4, " 
		"d.bindata from rcfile f join rcfiledata d on f.id = d.id " << whereClause;
	DBResult res = dbcon_->executeQuery(query.str(), 0, NULL, NULL, NULL, NULL);
	if (res.dataReturned()) {
		int numfiles = res.rowsReturned();
		for (int i=0; i < numfiles; i++) {
			uint32_t pid=0, pver=0, lastmod=0;
			string pname;
			char *ptr;
			ptr = res.getValue(i, 0);
			pid = ntohl(*(uint32_t*)ptr);
			ptr = res.getValue(i, 1);
			pver = ntohl(*(uint32_t*)ptr);
			pname = res.getValue(i, 2);
			ptr = res.getValue(i, 3);
			lastmod = ntohl(*(uint32_t*)ptr);
			int datalen = res.getLength(i, 4);
			char *data = res.getValue(i, 4);
			DBFileInfoPtr filePtr;
			if (filesById_.count(pid) > 0) {
				filePtr = filesById_.at(pid);
				filePtr->version = pver;
				filePtr->name = pname;
			} else {
				filePtr = DBFileInfoPtr(new DBFileInfo(pid, pver, pname));
				filesById_.insert(map<long,DBFileInfoPtr>::value_type(pid, filePtr));
			}
			//write data to disk
			fs::path filepath(_impl->workingDir_);
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
		LOG(WARNING) << "sql error: " << res.errorMessage() << endl;
	}
}


void
RC2::DBFileSource::insertOrUpdateLocalFile(long fileId, long wspaceId)
{
	if (_impl->wspaceId_ != wspaceId)
		return; //skip this file
	ostringstream where;
	where << "where f.id = " << fileId;
	loadFiles(where.str().c_str());
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
RC2::DBFileSource::insertDBFile(string fname)
{
	string filePath = _impl->workingDir_ + "/" + fname;
	LOG(INFO) << "insertDBFile(" << fname << ")" << endl;
	DBTransaction trans = dbcon_->startTransaction();
	long fileId = dbcon_->longFromQuery("select nextval('rcfile_seq'::regclass)");

	DBFileInfoPtr fobj(new DBFileInfo(fileId, 1, fname));
	if (stat(filePath.c_str(), &fobj->sb) == -1)
		throw runtime_error((format("stat failed for insert %s") % filePath).str());
	time_t modTime = fobj->sb.st_mtime;
	size_t newSize=0;
	unique_ptr<char[]> data = ReadFileBlob(filePath, newSize);
	
	string escapedName;
	dbcon_->escapeLiteral(fname, escapedName);
	ostringstream query;
	query << "insert into rcfile (id, version, wspaceid"
		<< ",name,filesize,lastmodified) values ("
		<< fileId << ", 1, " << _impl->wspaceId_ << "," << escapedName << "," << newSize 
		<< ", to_timestamp(" << modTime << "))";
	DBResult res1 = dbcon_->executeQuery(query.str());
	if (!res1.commandOK()) {
		LOG(INFO) << "insert dbfile failed: " << res1.errorMessage() << endl;
		throw FormattedException("failed to insert file %s: %s", fname.c_str(), res1.errorMessage());
	}
	query.clear();
	query.str("");
	query << "insert into rcfiledata (id, bindata) values (" << fileId << ", $1::bytea)";
	int pformats[] = {1};
	int pSizes[] = {(int)newSize};
	const char *params[1] = {data.get()};
	DBResult res2 = dbcon_->executeQuery(query.str(), 1, NULL, params, pSizes, pformats, 1);
	if (!res2.commandOK()) {
		LOG(INFO) << "executing query:" << query.str() << endl;
		LOG(INFO) << "insert dbfiledata failed: " << res2.errorMessage() << endl;
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
		throw runtime_error((format("stat failed for update %s") % fobj->name).str());
	time_t newMod = fobj->sb.st_mtime;
	size_t newSize=0;
	unique_ptr<char[]> data = ReadFileBlob(filePath, newSize);

	DBTransaction trans = dbcon_->startTransaction();
	ostringstream query;
	query << "update rcfile set version = " << newVersion << ", lastmodified = to_timestamp("
		<< newMod << "), filesize = " << newSize << " where id = " << fobj->id;
	LOG(INFO) << "executing " << query.str() << endl;
	DBResult res1 = dbcon_->executeQuery(query.str());
	if (!res1.commandOK()) {
		throw FormattedException("failed to update file %ld: %s", fobj->id, res1.errorMessage());
	}
	query.clear();
	query.str("");
	query << "update rcfiledata set bindata = $1::bytea where id = " << fobj->id;
	int pformats[] = {1};
	int pSizes[] = {(int)newSize};
	const char *params[] = {data.get()};
	DBResult res2 = dbcon_->executeQuery(query.str(), 1, NULL, params, pSizes, pformats);
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
	DBResult res = dbcon_->executeQuery(query.str());
	if (res.commandOK()) {
		filesById_.erase(fobj->id);
	} else {
		LOG(WARNING) << "sql error delting file " << fobj->id << ":" 
			<< res.errorMessage() << endl;
	}
}

