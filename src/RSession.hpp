#pragma once

#include <string>
#include <memory>
#include <stdint.h>
#include <functional>
#include <json/elements.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "common/RC2Utils.hpp"

extern const uint32_t kRSessionMagicNumber;

class RInside;

#define kError_Open_InvalidDir 101
#define kError_Open_CreateDirFailed 102
#define kError_Execfile_InvalidInput 103
#define kError_ExecFile_MarkdownFailed 104

typedef std::function<void(std::string&)> OutputCallback;
//typedef void (^OutputCallback)(std::string &str);

namespace RC2 {

	class RSessionCallbacks;
	class InputBufferManager;

	class RSession {

		public:
					RSession(RSessionCallbacks *callbacks);
			virtual ~RSession();
			
			bool 	parseArguments(int argc, char *argv[]);
			void	prepareForRunLoop();
			void	installExitHandler(event_callback_fn callback);

			bool isVerbose() const { return _verbose; }
			void setVerbose(bool v) { _verbose = v; }
			
			RSessionCallbacks* getCallbacks() const { return _callbacks; }
			void setCallbacks(RSessionCallbacks *cbs) { _callbacks = cbs; }

			std::string getWorkingDirectory() const { return _tmpDir->getPath(); }

			void	sendJsonToClientSource(std::string json);
/*
			//methods that should be private but are needed when unit testing
#ifndef UNITTESTING
		private:
#endif
			//the following are only public to allow for unit testing
//			std::string	handleIncomingData(dispatch_data_t data);
//			std::string handleIncomingOutput(dispatch_data_t data);
			void	sendToClient(std::string text, bool is_error=false);
			void	handleJsonCommand(std::string json);
			std::string	handleListVariablesCommand(bool delta, json::UnknownElement clientExtras);
			std::string	handleGetVariableCommand(std::string varName, std::string startTime);
			std::string	executeFile(std::string arg, std::string startTime, json::UnknownElement clientExtras);
			std::string	executeRMarkdown(std::string arg, std::string startTime, json::UnknownElement *clientExtras);
			std::string	executeSweave(std::string arg, std::string startTime, json::UnknownElement *clientExtras);
			void	setOutputFunction(OutputCallback callback);
*/			
		private:
/*			void	prepareForRead();
			void	flushOutputBuffer();
//			void	processOutput(bool done, dispatch_data_t data, int error, bool stderr);
			void	handleOpenCommand(std::string arg);
			void	handleHelpCommand(std::string arg, std::string startTime);
*/		
			std::string	formatStringAsJson(std::string &input, bool is_error);
			std::string acknowledgeExecComplete(std::string stime, json::UnknownElement *clientExtras=nullptr);
			void	addFileChangesToJson(JsonDictionary& json);
			void	clearFileChanges();
			void	handleJsonCommand(std::string json);
			void	handleOpenCommand(std::string arg);

			event_base*					_eventBase;
			struct bufferevent*			_eventBuffer;
			InputBufferManager*			_inputBuffer;
			RInside*					_R;
			std::unique_ptr<TemporaryDirectory>	_tmpDir;
//			KQueueFileWatcher*			_fileWatcher;
			RSessionCallbacks*		 	_callbacks;
//			dispatch_io_t				_clientSource;
//			dispatch_io_t				_stdoutSource;
//			dispatch_io_t				_stderrSource;
//			dispatch_data_t				_dataInProgress;
//			dispatch_queue_t			_queue;
			std::string					_outBuffer;
//			OutputCallback				_outFunction; //if no _clientSource, this is used (likely for testing)
			int							_socket;
			bool						_verbose;
			bool						_open;
			bool						_ignoreOutput;
			bool						_sourceInProgress;
			bool						_watchVariables;
	
	};
};
