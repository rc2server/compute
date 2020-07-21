#pragma once

#define kError_Unknown -1
#define kError_Open_InvalidDir 101
#define kError_Open_CreateDirFailed 102
#define kError_Execfile_InvalidInput 103
#define kError_ExecFile_MarkdownFailed 104
#define kError_UnknownFile 105
#define kError_MissingTransactionId 106
#define kError_MissingQueryId 107
#define kError_SweaveError 108
#define kError_SweaveErrorInLogFile 109
#define kError_InvalidArgument 110
#define kError_AlreadyExists 111
#define kError_QueryFailed 201
#define kError_QueryIncomplete 202
#define kError_QueryUnhandledResponse 203

#ifdef RINSIDE_RINSIDE_H

namespace RC2 {
	class RException: public std::exception {
		int code;
		Rcpp::RObject response;
	public:
		RException(int ecode, SEXP& answer)
			: code(ecode), response(answer)
			{}
		int getCode() const { return code; }
		SEXP getAnswer() const { return response; }
	};
};

#endif
