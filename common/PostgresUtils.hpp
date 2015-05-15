#include <postgresql/libpq-fe.h>
#include <string>
#include <stdlib.h>

class DBResult {
	PGresult *res;
	public:
		DBResult(PGresult* inRes) : res(inRes) {}
		DBResult(PGconn* con, std::string query) {
			res = PQexec(con, query.c_str());
		}
		DBResult(DBResult&& other) { //move constructor
			res = other.res;
			other.res = nullptr;
		}
		~DBResult() { PQclear(res); }
		DBResult& operator=(DBResult&& other) { //move assignment operator
			if (res)
				PQclear(res);
			res = other.res;
			other.res = nullptr;
			return *this;
		}
		operator PGresult*() const { return res; }

		bool commandOK() const {
			ExecStatusType rc = PQresultStatus(res);
			return PGRES_COMMAND_OK == rc;
		}
		bool dataReturned() const {
			ExecStatusType rc = PQresultStatus(res);
			return PGRES_TUPLES_OK == rc;
		}
		const char *errorMessage() {
			return PQresultErrorMessage(res);
		}
		int rowsAffected() const {
			return atoi(PQcmdTuples(res));
		}
};

class DBTransaction {
	PGconn *con_;
	bool open_;
	public:
		DBTransaction(PGconn *con) : con_(con), open_(true) {
			DBResult res(con_, "begin");
		}
		~DBTransaction() {
			if (open_)
				DBResult(con_, "rollback");
		}
		DBResult commit() {
			DBResult res(con_, "commit");
			open_ = !res.commandOK();
			return res;
		}
};
