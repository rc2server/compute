#include <sstream>
#include "PGDBConnection.hpp"


RC2::PGDBConnection::PGDBConnection()
	: dbcon_(nullptr)
{

}

RC2::PGDBConnection::~PGDBConnection()
{
	if (dbcon_) {
		PQfinish(dbcon_);
	}
}

void RC2::PGDBConnection::connect ( std::string connectStr ) {
	if (connectStr.length() < 5)
		throw DBException("invalid connectString");
	dbcon_ = PQconnectdb(connectStr.c_str());
	if (NULL == dbcon_) {
		throw DBException("PQconnectdb returned NULL");
	} else if (PQstatus(dbcon_) != CONNECTION_OK) {
		std::ostringstream errorBuffer;
		errorBuffer << "failed to connect to database: " << PQerrorMessage(dbcon_);
		throw DBException(errorBuffer.str());
	}
}

DBResult RC2::PGDBConnection::executeQuery ( std::string query )
{
	return DBResult(PQexec(dbcon_, query.c_str()));
}

long int RC2::PGDBConnection::longFromQuery ( std::string query )
{
	long value = 0;
	DBResult res(PQexec(dbcon_, query.c_str()));
	if (res.dataReturned()) {
		value = atol(PQgetvalue(res, 0, 0));
	} else {
		throw DBException(res.errorMessage());
	}
	return value;
}

DBResult RC2::PGDBConnection::executeQuery ( std::string query, int numParams, 
											 const Oid* paramTypes, const char*const* paramValues, 
											 const int* paramLengths, const int* paramFormats, 
											 int resultFormat )
{
	return DBResult(PQexecParams(dbcon_, query.c_str(), numParams, paramTypes, paramValues, paramLengths, paramFormats, resultFormat));
}

bool RC2::PGDBConnection::checkForNotification(std::string& channel, std::string& parameter)
{
	PGnotify *notify;
	bool result = false;
	if ((notify = PQnotifies(dbcon_)) != NULL) {
		channel = notify->relname;
		parameter = notify->extra;
		PQfreemem(notify);
		return true;
	}
	return false;
}
