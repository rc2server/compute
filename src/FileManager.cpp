#include "FileManager.hpp"
#include <iostream>
#include <postgresql/libpq-fe.h>
#include <glog/logging.h>

using namespace std;

class RC2::FileManager::Impl {
	public:
		PGconn*		dbcon;

		void cleanup(); //replacement for destructor
		void connect(string str, int wspaceId);
};

void
RC2::FileManager::Impl::cleanup() {
	if (dbcon)
		PQfinish(dbcon);
}

void
RC2::FileManager::Impl::connect(string str, int wspaceId) {
	dbcon = PQconnectdb(str.c_str());
	char msg[255];
	sprintf(msg, "select name, id from rcfile where wspaceid = %d", wspaceId);
	cerr << "executing: " << msg << endl;
	PGresult *res = PQexec(dbcon, msg);
	ExecStatusType rc = PQresultStatus(res);
	if (PGRES_TUPLES_OK == rc) {
		int numfiles = PQntuples(res);
		for (int i=0; i < numfiles; i++) {
			cerr << " file " << PQgetvalue(res, i, 0);
		}
	} else {
		cerr << "sql error: " << PQresultErrorMessage(res) << endl;
	}
	PQclear(res);
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
