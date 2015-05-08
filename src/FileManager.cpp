#include "FileManager.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <event2/bufferevent.h>
#include <postgresql/libpq-fe.h>
#include <glog/logging.h>
#include <libpqtypes.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <sys/types.h>
#include <utime.h>
#include "../common/FormattedException.hpp"

using namespace std;
using boost::format;
namespace fs = boost::filesystem;

#undef LOG
#define LOG(x) cerr

//for db fetched data
struct DBFileInfo {
	long	id, version;
	string	name;
	
	DBFileInfo(long anId, long aVersion, const char *aName) : id(anId), version(aVersion), 
		name(aName) 
	{}
	
	friend ostream& operator<<(ostream &out, DBFileInfo &finfo) {
		out << "(" << finfo.id << ": " << finfo.name << ")";
		return out;
	}
};

//for inotify
struct FSObject 
{
	string			name;
	struct stat 	sb;
	int				wd;
	inline bool is_dir() const { return S_ISDIR(sb.st_mode); }
};


class RC2::FileManager::Impl {
	public:
		PGconn*				dbcon;
		vector<DBFileInfo>	wsFiles;
		vector<DBFileInfo>	pFiles;
		string				workingDir;
		struct event_base*	eventBase;
		int					inotifyFd;
		map<int, FSObject>	files;
		FSObject			root;

		void cleanup(); //replacement for destructor
		void connect(string str, long wspaceId);
		void loadFiles(string fileType, long typeId, string subfolder, vector<DBFileInfo> &files);
		void insertDBFile(string fname);
		void updateDBFile(FSObject &fobj);
		void removeDBFile(FSObject &fobj);
		long query1long(const char *query);
		
		void	setupInotify(FileManager *fm);
		void 	watchFile(string fname);
		void	handleInotifyEvent(struct bufferevent *bev);
		static void handleInotifyEvent(struct bufferevent *bev, void *ctx)
		{
			FileManager *watcher = reinterpret_cast<FileManager*>(ctx);
			watcher->_impl->handleInotifyEvent(bev);
		}

	class StatException : public runtime_error {
		using runtime_error::runtime_error;
	};
};

void
RC2::FileManager::Impl::cleanup() {
	if (inotifyFd != -1)
		close(inotifyFd);
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
	vector<DBFileInfo> &files)
{
	PGparam *param = PQparamCreate(dbcon);
	string query = "select f.id, f.version, f.name, f.datecreated, f.lastmodified, " 
		"d.bindata from rcfile f join rcfiledata d on f.id = d.id where f." + fileType + " = $1";
	if(!PQputf(param, "%int4", typeId))
		LOG(ERROR) << "prep error:" << PQgeterror() << endl;
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
				LOG(ERROR) << "error:" << PQgeterror() << endl;
			files.push_back(DBFileInfo(pid, pver, pname));
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
		LOG(ERROR) << "sql error: " << PQresultErrorMessage(res) << endl;
	}
	PQclear(res);
}

void 
RC2::FileManager::Impl::insertDBFile(string fname) {
	cerr << "insert file:" << fname << endl;
}

void 
RC2::FileManager::Impl::updateDBFile(FSObject &fobj) {
	cerr << "update file:" << fobj.name << endl;
}

void 
RC2::FileManager::Impl::removeDBFile(FSObject &fobj) {
	cerr << "rm file:" << fobj.name << endl;
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
		LOG(ERROR) << "sql error:" << PQresultErrorMessage(res) << endl;
	}
	PQclear(res);
	return value;
}

#pragma mark -

void
RC2::FileManager::Impl::setupInotify(FileManager *fm) {
	DIR *dir;
	class dirent *ent;
	inotifyFd = inotify_init();
	if (inotifyFd == -1)
		throw FormattedException("inotify_init() failed for %s", workingDir.c_str());
	root.wd = inotify_add_watch(inotifyFd, workingDir.c_str(), 
		IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF | IN_MOVE);
	if (root.wd == -1)
		throw FormattedException("inotify_add_watched failed");
	stat(workingDir.c_str(), &root.sb);
	files[root.wd] = root;
	
	dir = opendir(workingDir.c_str());
	while ((ent = readdir(dir)) != NULL) {
		//skip "." and ".." but not other files that start with a period
		if (ent->d_name[0] == '.' && (strlen(ent->d_name) == 1 || ent->d_name[1] == '.'))
			continue;
		try {
			watchFile(ent->d_name);
		} catch (Impl::StatException &se) {
		}
	}
	closedir(dir);
	
	struct bufferevent *evt = bufferevent_socket_new(eventBase, inotifyFd, 0);
	bufferevent_setcb(evt, FileManager::Impl::handleInotifyEvent, NULL, NULL, fm);
	bufferevent_enable(evt, EV_READ);
}

#define I_BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void
RC2::FileManager::Impl::handleInotifyEvent(struct bufferevent *bev)
{
	LOG(INFO) << "handleInotify called:" << workingDir << endl;
	char buf[I_BUF_LEN];
	size_t numRead = bufferevent_read(bev, buf, I_BUF_LEN);
	char *p;
	for (p=buf; p < buf + numRead; ) {
		struct inotify_event *event = (struct inotify_event*)p;
		int evtype = event->mask & 0xffff; //events are in lower word, flags in upper
		LOG(INFO) << "notify:" << std::hex << evtype << " for " << 
			event->wd << endl;
		if(evtype == IN_CREATE) {
			insertDBFile((string)event->name);
			watchFile(event->name);
			//need to add a watch for this file
		} else if (evtype == IN_CLOSE_WRITE) {
			updateDBFile(files[event->wd]);
		} else if (evtype == IN_DELETE_SELF) {
			removeDBFile(files[event->wd]);
			//discard our records of it
			inotify_rm_watch(inotifyFd, event->wd);
			files.erase(event->wd);
		}
		//handle event
		p += sizeof(struct inotify_event) + event->len;
	}
}

void 
RC2::FileManager::Impl::watchFile(string fname) {
	FSObject fo;
	fo.name = fname;
	string fullPath = workingDir + "/" + fo.name;
	if (stat(fullPath.c_str(), &fo.sb) == -1)
		throw StatException((format("stat failed for %s") % fo.name).str());
	fo.wd = inotify_add_watch(inotifyFd, fullPath.c_str(), 
				IN_CLOSE_WRITE | IN_DELETE_SELF | IN_MODIFY);
	if (fo.wd == -1)
		throw FormattedException("inotify_add_warched failed %s", fo.name.c_str());
	files[fo.wd] = fo;
	LOG(INFO) << "watching " << fo.name << endl;
}


#pragma mark -


RC2::FileManager::FileManager()
	: _impl(new Impl())
{
	_impl->eventBase = nullptr;
	_impl->inotifyFd = -1;
}

RC2::FileManager::~FileManager()
{
	_impl->cleanup();
}

void
RC2::FileManager::initFileManager(string connectString, int wspaceId)
{
	_impl->connect(connectString, wspaceId);
	_impl->setupInotify(this);
}

void
RC2::FileManager::setWorkingDir(std::string dir) 
{ 
	_impl->workingDir = dir; 
}

void
RC2::FileManager::setEventBase(struct event_base *eb)
{
	_impl->eventBase = eb;
}
