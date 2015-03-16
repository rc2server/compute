#pragma once

#include <string>
#include <stdlib.h>
#include "json/elements.h"

namespace RC2 {

std::string GenerateUUID();
std::string GetPathForExecutable(pid_t pid);
std::string SlurpFile(const char *filename);
std::runtime_error FormatErrorAsJson(int errorCode, std::string details);

//returns the return code from last call to mkdir
int MakeDirectoryPath(std::string s, mode_t mode);

class TemporaryDirectory {
	public:
					TemporaryDirectory();
					TemporaryDirectory(std::string path, bool erase=false);
		virtual		~TemporaryDirectory();
		
		std::string	getPath() const { return _path; }
	
	private:
		std::string		_path;
		bool			_eraseOnDeath;

};

class JsonDictionary {
	public:
					JsonDictionary();
	virtual			~JsonDictionary();
	
					operator std::string () const;
			std::string	string() const;
	
			void 	addString(std::string key, std::string value);
			void	addBool(std::string key, bool value);
			void	addInt(std::string key, int value);
			void	addObject(std::string key, json::Object &value);
			std::string	addStringArray(std::string key, std::vector<std::string> strings);
	
	private:
		json::Object	_doc;
};

};
