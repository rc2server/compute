#pragma once

#include <string>
#include <memory>
#include <algorithm>
#include <stdlib.h>

namespace RC2 {

std::string GenerateUUID();
std::string GetPathForExecutable(pid_t pid);
std::string SlurpFile(const char *filename);
std::runtime_error FormatErrorAsJson(int errorCode, std::string details);
std::unique_ptr<char[]> ReadFileBlob(std::string filePath, size_t &size);
inline void StripQuotes(std::string& str) {
	str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
}

//returns the return code from last call to mkdir
int MakeDirectoryPath(std::string s, mode_t mode);

class TemporaryDirectory {
	public:
					TemporaryDirectory(bool erase=true);
					TemporaryDirectory(std::string path, bool erase=false);
		virtual		~TemporaryDirectory();
		
		std::string	getPath() const { return _path; }
	
	private:
		std::string		_path;
		bool			_eraseOnDeath;

};

struct BooleanWatcher {
	BooleanWatcher(bool* ptr)
	:_ptr(ptr)
	{
		*_ptr = true;
	}
	~BooleanWatcher() { 
		*_ptr = false;
	}
private:
	bool* _ptr;
};

};
