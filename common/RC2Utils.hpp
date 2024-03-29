#pragma once

#include <string>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <stdlib.h>
#include <signal.h>
#include <functional>
#include <boost/crc.hpp>

namespace RC2 {

std::string GenerateUUID();
std::string GetPathForExecutable(pid_t pid);

/**
 * @brief wrapper around arbitrary bytes 
 */
struct BinaryData {
	BinaryData(int inSize = 0) : size(inSize)
	{
		if (size > 0)
			data = std::move(std::make_unique<char[]>(size));
	}

	size_t getSize() const { return size; }
	void setSize(size_t newSize) {
		size = newSize;
		data = std::make_unique<char[]>(size);
	}
	
	std::unique_ptr<char[]> data;
private:
	size_t size;
};

/**
 * @brief Reads a file, does a find/replace, and then saves the modified file
 * 
 * @param filename path to the file to edit
 * @param searchString text to search for
 * @param replaceString text to replace searchString with
 */
void UpdateFile(const char *filename, std::string searchString, std::string replaceString);

/**
* @brief Reads the contents of the specified file
* 
* @param filename the name of the file to read relative to the working directory
* @return std::string the contents of the file
* @throw boost filesystem_error
*/
std::string SlurpFile(const char *filename);

/**
 * @brief reads the contents of filename to data
 * 
 * @param filename the name/path of the file to read
 * @param data object to store the read bytes
 * @throw boost filesystem_error
 */
void SlurpBinaryFile(std::string filename, BinaryData& data);

std::string SHA1Hash(std::string& input);
std::string SHA256Hash(const std::string input);

inline int calculateCRCChecksum(const std::string& instr) {
	boost::crc_32_type result;
	result.process_bytes(instr.data(), instr.length());
	return result.checksum();
}

void RemoveFile(std::string filename);

std::string PrivatePackagePath();
std::runtime_error FormatErrorAsJson(int errorCode, std::string details);
std::unique_ptr<char[]> ReadFileBlob(std::string filePath, size_t &size);
inline void StripQuotes(std::string& str) {
	str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
}

/** @brief returns the return code from last call to mkdir */
int MakeDirectoryPath(std::string s, mode_t mode);

class TemporaryDirectory {
	public:
					TemporaryDirectory(bool erase=true);
					/** @brief path will be created if it doesn't exist */
					TemporaryDirectory(std::string path, bool erase=false);
		virtual		~TemporaryDirectory();
		
		std::string	getPath() const { return _path; }
	
	private:
		std::string		_path;
		bool			_eraseOnDeath;

};

//use as
// Defer onExit([&usedVar, this](void) {
//     free(foo); //or whatever code to run when out of scope
// });
struct Defer {
	Defer(std::function<void (void)> f) : f_(f) {}
	~Defer(void) { f_(); }
private:
	std::function<void (void)> f_;
};

/** temporarily suspend a signal. Useful with a fork call to prevent race conditions */
struct SignalSuspender {
	SignalSuspender(int signal) {
		sigset_t mask;
		sigemptyset(&mask);
		sigaddset(&mask, signal);
		if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
			throw std::runtime_error("sigproc mask failed");
	}
	
	~SignalSuspender() {
		sigset_t mask;
		sigemptyset(&mask);
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
	}
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
