#pragma once

#include <string>
#include <memory>
#include <stdint.h>
#include <functional>
#include <json/elements.h>
#include <boost/noncopyable.hpp>

using std::string;

extern const uint32_t kRSessionMagicNumber;

class RInside;

#define kError_Open_InvalidDir 101
#define kError_Open_CreateDirFailed 102
#define kError_Execfile_InvalidInput 103
#define kError_ExecFile_MarkdownFailed 104

typedef std::function<void(string&)> OutputCallback;
//typedef void (^OutputCallback)(string &str);

namespace RC2 {

	class RSessionCallbacks;
	class InputBufferManager;

	class RSession : private boost::noncopyable {

		public:
					RSession(RSessionCallbacks *callbacks);
			virtual ~RSession();
			
			bool 	parseArguments(int argc, char *argv[]);
			void	prepareForRunLoop();
			void	installExitHandler(void(*)(short flags));
			void	startEventLoop();

			RSessionCallbacks* getCallbacks() const { return _callbacks; }
			void setCallbacks(RSessionCallbacks *cbs) { _callbacks = cbs; }

			string getWorkingDirectory() const;

			//unit test subclasses might override
			virtual void	sendJsonToClientSource(string json);

		protected:
			void	consoleCallback(const string &text, bool is_error);
			string	formatStringAsJson(const string &input, bool is_error);
			void	clearFileChanges();
			void	flushOutputBuffer();
			void	sendOutputBufferToClient(bool is_error);
			void	sendTextToClient(string text, bool is_error=false);
			void	handleJsonCommand(string json);
			void	handleOpenCommand(string arg);
			void	handleHelpCommand(string arg, string startTime);
			string	handleListVariablesCommand(bool delta, json::UnknownElement clientExtras);
			string	handleGetVariableCommand(string varName, string startTime);

			string	executeFile(long fileId, string startTime, json::UnknownElement clientExtras);
			string	executeRMarkdown(string arg, long fileId, string startTime, json::UnknownElement *clientExtras);
			string	executeSweave(string arg, long fileId, string startTime, json::UnknownElement *clientExtras);

			void	scheduleExecCompleteAcknowledgmenet(string stime, 
						json::UnknownElement *clientExtras=nullptr);
			static void handleJsonStatic(struct bufferevent *bev, void *ctx);

			struct Impl;
			std::unique_ptr<Impl>		_impl;
			RSessionCallbacks*		 	_callbacks;
				
	};
};
