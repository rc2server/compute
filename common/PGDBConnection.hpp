#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include "PostgresUtils.hpp"

namespace RC2 {

class PGDBConnection {

public:
	PGDBConnection();
	virtual ~PGDBConnection();
	
	/// throws a DBException on failure to connect
	virtual void connect(std::string connectStr);
	virtual bool connectionOpen() const { return dbcon_ != nullptr && PQstatus(dbcon_) == CONNECTION_OK; }
	
	virtual DBResult executeQuery(std::string query);
	///throws DBException if query fails
	virtual long longFromQuery(std::string query);
	
	DBResult executeQuery(std::string query, int numParams, const Oid *paramTypes, 
						  const char *const* paramValues, const int *paramLengths, 
					   const int *paramFormats, int resultFormat = 1);
	
	DBTransaction startTransaction() { return DBTransaction(dbcon_); }
	
	void escapeLiteral(std::string inString, std::string &outString) {
		auto ptr = PQescapeLiteral(dbcon_, inString.c_str(), inString.length());
		outString = ptr;
		PQfreemem(ptr);
	}
	
	void consumeInput() { if (PQconsumeInput(dbcon_) != 1) throw DBException("consume input returned error"); }
	bool checkForNotification(std::string &channel, std::string &parameter);
	
	int getSocket() { return PQsocket(dbcon_); }
	
private:
	//disable copying so move semantics are used
	PGDBConnection(const PGDBConnection&);
	PGDBConnection& operator=(const PGDBConnection&);
	
protected:
	PGconn *dbcon_;
};
	
};
