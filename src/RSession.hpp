#pragma once

#include <string>
#include <memory>
#include <stdint.h>
#include <functional>
#include <boost/noncopyable.hpp>
#include "JsonCommand.hpp"
#include "SessionCommon.hpp"
#include "Errors.hpp"
#include "PreviewData.hpp"

using std::string;

extern const uint32_t kRSessionMagicNumber;

class RInside;

typedef std::function<void(string&)> OutputCallback;
//typedef void (^OutputCallback)(string &str);

namespace RC2 {

	class RSessionCallbacks;
	class InputBufferManager;
	class FileManager;
	struct FileInfo;

	class RSession : private boost::noncopyable, public PreviewDelegate {

		public:
			/**
			 * @brief creates a session object that encapsulates a instance of R
			 * 
			 * @param callbacks The callbacks used for R output while a command is executing
			 */
			explicit RSession(RSessionCallbacks *callbacks);
			virtual ~RSession();
			
			/**
			 * @brief parses -s specifying the socket to listen to,  -v to enable logging,
			 * and -d int to specify the delay in seconds before the event loop is started,
			 * which is useful for attaching the debugger after launch. Returns true if 
			 * successfully parsed
			 * 
			 * @param argc The number of arguments
			 * @param argv the command line arguments
			 * @return bool
			 */
			bool 	parseArguments(int argc, char *argv[]);
			/**
			 * @brief sets up the libevent event loop
			 * 
			 * @return void
			 */
			void	prepareForRunLoop();
			
			/**
			 * @brief installs a signal handler in the event loop. NOT IMPLEMENTED
			 * 
			 * @param  the signal handler
			 * @return void
			 */
			/**
			 * @brief starts the libevent event loop
			 * 
			 * @return void
			 */
			void	installExitHandler(void(*)(short flags));
			
			/**
			 * @brief starts the libevent event loop
			 * 
			 * @return void
			 */
			virtual void	startEventLoop();
			/**
			 * @brief stops the libevent event loop
			 * 
			 * @return void
			 */
			virtual void	stopEventLoop();

			/**
			 * @brief returns the current output callbacks
			 * 
			 * @return RC2::RSessionCallbacks*
			 */
			RSessionCallbacks* getCallbacks() const { return _callbacks; }
			/**
			 * @brief sets the callbacks used for R output during a command
			 * 
			 * @return void
			 */
			void setCallbacks(RSessionCallbacks *cbs) { _callbacks = cbs; }

			/**
			 * @brief returns the current working directory
			 * 
			 * @return string
			 */
			string getWorkingDirectory() const;

			/**
			 * @brief returns the current apiVersion
			 * 
			 * @return api version
			*/
			int getApiVersion() const;
			
			//unit test subclasses might override
			
			/**
			 * @brief process a json command just like if it was received over the network
			 * 
			 * @param json the json to parse as a command
			 * @return void
			 */
			virtual void sendJsonToClientSource(string json);

			/**
			 *  @brief execute a command with console output suppressed
			 *
			 *	@param code The code to execute
			 *	@param answer The returned value from the code
			 * 	@param env The environment to execute the code in
			 * @throws if query fails, RException with error and result. Otherwise GenericException with message
			 */ 
			virtual void executeNonUserCode(string code, SEXP& answer, Rcpp::Environment* env);

			virtual void sendPreviewJson(string jsonStr);
			virtual void executePreviewCode(string code, SEXP& result, Rcpp::Environment* environment);
			
			
		protected:
			RInside* getRInside() const;
			void	consoleCallback(const string &text, bool is_error);
			string	formatStringAsJson(const string &input, bool is_error);
			void	clearFileChanges();
			void	flushOutputBuffer();
			void	sendOutputBufferToClient(bool is_error);
			void	sendTextToClient(string text, bool is_error=false);
			void	handleJsonCommand(string json);
			void	handleCommand(JsonCommand& command);
			void	handleOpenCommand(JsonCommand& command);
			void	handleCloseCommand();
			void	handleSaveEnvCommand();
			void	handleHelpCommand(JsonCommand& command);
			void	handleListVariablesCommand(bool delta, JsonCommand& command);
			void	handleGetVariableCommand(JsonCommand& command);
			void	handleClearEnvironment(JsonCommand& command);
			void	handleCreateEnvironment(JsonCommand& command);
			void	handleInitPreview(JsonCommand& command);
			void	handleUpdatePreview(JsonCommand& command);
			void	handleRemovePreview(JsonCommand& command);

			void	handleExecuteScript(JsonCommand& command);
			void	handleExecuteAndReturn(JsonCommand& command);
			void	executeFile(JsonCommand& command);
			void	executeRMarkdown(string filePath, long fileId, JsonCommand& command);
			void	executeSweave(string filePath, long fileId, JsonCommand& command);

			void	scheduleExecCompleteAcknowledgment(JsonCommand& command, int queryId, FileInfo *info=nullptr);
			void	scheduleDelayedCommand(string json);
			static void handleJsonStatic(struct bufferevent *bev, void *ctx);

			//this is likely needed for subclasses (like in a unit test)
			//caller will have to cast to event_base*
			struct event_base*	getEventBase() const;
			ExecuteCallback getExecuteCallback();
			bool loadEnvironment();
			
			//tests need to be able to set this
			void setApiVersion(int);
			
			// testing subclasses need to override this since the executable 
			// directory is not the same as rsession
			virtual string incomingJsonSchemaPath();
			
			FileManager const* getFileManager() const; //should not be deallocated
			void setFileManager(FileManager *manager);

			struct Impl;
			std::unique_ptr<Impl>			_impl;
			RSessionCallbacks*		 		_callbacks;

	};
};
