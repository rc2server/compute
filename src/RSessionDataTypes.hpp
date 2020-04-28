#pragma once
#include "EnvironmentWatcher.hpp"
#include "PreviewData.hpp"
#include <memory>
#include <map>
#include <string>
#include <boost/filesystem.hpp>

using std::string;

namespace fs = boost::filesystem;

namespace RC2 {
	typedef std::map<long, std::shared_ptr<EnvironmentWatcher>> EnvironmentMap;
	typedef std::map<int, std::unique_ptr<PreviewData>> PreviewMap;

	enum PdfStatus { successPdfStatus, killedPdfStatus, otherPdfStatus };
	
	struct ExecCompleteArgs {
		RSession *session;
		JsonCommand command;
		FileInfo finfo;
		int queryId;
		//object ptr points to does not have to exist past this call. just to allow null value
		ExecCompleteArgs(RSession *inSession, JsonCommand inCommand, int inQueryId, FileInfo *info) 
		: session(inSession), command(inCommand), finfo(info), queryId(inQueryId)
		{}
	};
	
	struct DelayCommandArgs {
		RSession *session;
		string json;
		DelayCommandArgs(RC2::RSession *inSession, string inJson)
		: session(inSession), json(inJson)
		{}
	};
	
	struct BoolResetter {
		BoolResetter(bool *ptr, bool valueToSet) { _ptr = ptr; _valToSet = valueToSet; *_ptr = valueToSet; }
		~BoolResetter() { *_ptr = !_valToSet; }
	private:
		bool *_ptr;
		bool _valToSet;
	};
	
	struct SweavePdfData {
		JsonCommand command;
		RSession *session;
		pid_t pid;
		fs::path srcPath;
		fs::path scratchPath;
		string baseName;
		int queryId;
		PdfStatus pstatus = successPdfStatus;
		SweavePdfData (JsonCommand cmd, RSession *s, pid_t p, fs::path spath, fs::path tpath, string name, int qid = 0) 
		: command(cmd), session(s), pid(p), srcPath(spath), scratchPath(tpath), baseName(name), queryId(qid)
		{};
	};
	
} // namespace


inline bool stringHasPrefix(string str, const char* prefix) {
	return strncmp(str.c_str(), prefix, strlen(prefix)) == 0;
}

inline double currentFractionalSeconds() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (round(tv.tv_usec/1000.0)/1000.0);
}

enum KnitExceptionCode { none, forkError=100, execvError, pathError, pdfError, errorInLogFile, unknownError };

inline int clientErrorForKnitExceptionCode(KnitExceptionCode code) {
	switch (code) {
		case forkError:
		case execvError:
		case pathError:
		case pdfError:
			return kError_SweaveError;			
		case errorInLogFile:
			return kError_SweaveErrorInLogFile;
		case unknownError:
		default:
			return kError_Unknown;
	}
}
