#pragma once

#include <string>
#include <memory>
#include <stdlib.h>

namespace RC2 {

std::string GenerateUUID();
std::string GetPathForExecutable(pid_t pid);
std::string SlurpFile(const char *filename);
std::runtime_error FormatErrorAsJson(int errorCode, std::string details);
std::unique_ptr<char[]> ReadFileBlob(std::string filePath, size_t &size);

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

};
