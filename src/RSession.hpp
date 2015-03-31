#pragma once

#include <string>
#include <memory>
#include <stdint.h>
#include <functional>
#include <json/elements.h>
#include <boost/noncopyable.hpp>

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

			std::string getWorkingDirectory() const;

			//unit test subclasses might override
			virtual void	sendJsonToClientSource(std::string json);
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
		protected:
/*			void	prepareForRead();
//			void	processOutput(bool done, dispatch_data_t data, int error, bool stderr);
			void	handleOpenCommand(std::string arg);
			void	handleHelpCommand(std::string arg, std::string startTime);
*/		
			std::string	formatStringAsJson(const std::string &input, bool is_error);
			std::string acknowledgeExecComplete(std::string stime, json::UnknownElement *clientExtras=nullptr);
			void	addFileChangesToJson(JsonDictionary& json);
			void	clearFileChanges();
			void	flushOutputBuffer();
			void	sendTextToClient(std::string text, bool is_error=false);
			void	handleJsonCommand(std::string json);
			void	handleOpenCommand(std::string arg);


			struct Impl;
			std::unique_ptr<Impl>		_impl;
			RSessionCallbacks*		 	_callbacks;
				
	};
};
