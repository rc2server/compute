#pragma once
#include <nlohmann/json.hpp>

using json2 = nlohmann::json;
namespace RC2 {
	
	enum class CommandType {
		Unknown=-1, Open, Close, ClearFileChanges, ExecScript, ExecAndReturn, ExecFile,
		Help, ListVariables, GetVariable, ToggleWatch, SaveData, ClearEnvironment, CreateEnvironment,
		InitPreview, UpdatePreview, RemovePreview
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
				if (cmdStr == "execAndReturn") _type = CommandType::ExecAndReturn;
				if (cmdStr == "execFile") _type = CommandType::ExecFile;
				if (cmdStr == "help") _type = CommandType::Help;
				if (cmdStr == "listVariables") _type = CommandType::ListVariables;
				if (cmdStr == "getVariable") _type = CommandType::GetVariable;
				if (cmdStr == "toggleVariableWatch") _type = CommandType::ToggleWatch;
				if (cmdStr == "clearEnvironment") _type = CommandType::ClearEnvironment;
				if (cmdStr == "createEnvironment") _type = CommandType::CreateEnvironment;
				if (cmdStr == "initPreview") _type = CommandType::InitPreview;
				if (cmdStr == "updatePreview") _type = CommandType::UpdatePreview;
				if (cmdStr == "removePreview") _type = CommandType::RemovePreview;
			}
			
			CommandType type() const { return _type; }
			json2::value_type raw() const { return _cmd; }
			int apiVersion() { return _cmd.value("apiVersion", 0); }
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
			bool valueIsNull(std::string key) const {
				return _cmd.count(key) == 0 || _cmd[key].is_null();
			}
			json2::string_t valueForKey(std::string key, std::string defaultValue = "") const {
				if (_cmd.count(key) == 0) return defaultValue;
				if (_cmd[key].is_string()) return _cmd[key];
				return defaultValue;
			}
			bool boolValueForKey(std::string key, bool defaultValue = false) const {
				if (_cmd.count(key) == 0) return defaultValue;
				if (_cmd[key].is_boolean()) return _cmd[key];
				return defaultValue;
			}
			
			int intValueForKey(std::string key, int defaultValue = 0) const {
				if (_cmd.count(key) == 0) return defaultValue;
				if (_cmd[key].is_number_integer()) return _cmd[key];
				return defaultValue;
			}
			long envId() const {
				return _cmd.value("contextId", 0);
			}
		
	};
	
};
