#pragma once
#include "json.hpp"

using json2 = nlohmann::json;
namespace RC2 {
	
	enum class CommandType {
		Unknown=-1, Open, Close, ClearFileChanges, ExecScript, ExecFile,
		Help, ListVariables, GetVariable, ToggleWatch, SaveData, ClearEnvironment
	};
	
	class JsonCommand {
		json2::value_type _cmd;
		CommandType _type;
	public:
		
		JsonCommand(json2::value_type cmd)
			: _cmd(cmd)
			{
				std::string cmdStr = cmd["msg"];
				if (cmdStr == "open") _type = CommandType::Open;
				if (cmdStr == "close") _type = CommandType::Close;
				if (cmdStr == "saveEnv") _type = CommandType::SaveData;
				if (cmdStr == "clearFileChanges") _type = CommandType::ClearFileChanges;
				if (cmdStr == "execScript") _type = CommandType::ExecScript;
				if (cmdStr == "execFile") _type = CommandType::ExecFile;
				if (cmdStr == "help") _type = CommandType::Help;
				if (cmdStr == "listVariables") _type = CommandType::ListVariables;
				if (cmdStr == "getVariable") _type = CommandType::GetVariable;
				if (cmdStr == "toggleVariableWatch") _type = CommandType::ToggleWatch;
				if (cmdStr == "clearEnvironment") _type = CommandType::ClearEnvironment;
			}
			
			CommandType type() const { return _type; }
			json2::value_type raw() const { return _cmd; }
			std::string argument() const { return _cmd.value("argument", ""); }
			std::string startTimeStr() const {
				return _cmd.value("startTime", ""); 
			}
			bool watchVariables() const { 
				return _cmd.value("watchVariables", false); 
			}
			json2::value_type clientData() const { 
				if (_cmd.find("clientData") == _cmd.end()) 
					return json2::parse("{}");
				return _cmd["clientData"]; 
				
			}
			json2::string_t valueForKey(std::string key) {
				if (_cmd[key].is_null()) return "";
				try { 
					return _cmd[key];
				} catch (std::out_of_range &oe) {
					return "";
				}
			}
			long envId() const {
				return _cmd.value("contextId", 0);
			}
		
	};
	
};
