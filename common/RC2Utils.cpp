#include "RC2Utils.hpp"
#include <unistd.h>
#include <uuid/uuid.h>
#if defined(__APPLE__)
#include <libproc.h>
#else
typedef char* uuid_string_t;
#endif
#include <sys/stat.h>
#include <system_error>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <openssl/sha.h>
#include <boost/filesystem.hpp>
#include <boost/exception/exception.hpp>
#include <boost/system/error_code.hpp>

namespace fs = boost::filesystem;
namespace errc =  boost::system::errc;

RC2::TemporaryDirectory::TemporaryDirectory(bool erase)
{
	_eraseOnDeath = erase;
	std::string uuid = GenerateUUID();
	_path = "/tmp/" + uuid;
	if (mkdir(_path.c_str(), 0700) != 0) {
		throw std::runtime_error("failed to create directory:" + _path);
	}
}

RC2::TemporaryDirectory::TemporaryDirectory(std::string path, bool erase)
	: _path(path), _eraseOnDeath(erase)
{
	bool exists = fs::exists(path);
	if (!exists && !fs::is_directory(path))
		throw std::runtime_error("path exists and is not a directory: " + path);
	if (!exists && RC2::MakeDirectoryPath(_path, 0700) != 0) {
		throw std::runtime_error("failed to create directory:" + _path);
	}
}

RC2::TemporaryDirectory::~TemporaryDirectory()
{
	if (_eraseOnDeath)
		fs::remove_all(_path);
}

std::string
RC2::SHA256Hash(std::string& input) 
{
	unsigned char obuf[SHA256_DIGEST_LENGTH + 1];
	bzero(obuf, SHA256_DIGEST_LENGTH + 1);
	SHA256((unsigned char*)input.data(), input.length(), obuf);
	return std::string((char*)obuf);
}

std::string 
RC2::SHA1Hash(std::string& input)
{
	// only needs to be 20, but we'll add one for terminator and clear entire buffer
	unsigned char obuf[21];
	bzero(obuf, 21);
	SHA1((unsigned char*)input.data(), input.length(), obuf);
	return std::string((char*)obuf);
}


std::string
RC2::SlurpFile(const char *filename)
{
	// boost calls will throw filesystem_errors
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		fs::path filePath(filename);
		boost::system::error_code code;
		auto fileStatus = fs::status(filePath, code);
		auto exists = fs::exists(filePath);
		auto regular = fs::is_regular_file(filePath);
		assert(exists && regular);
		std::string contents;
		in.seekg(0, std::ios::end);
		int len = fs::file_size(filePath);
		contents.resize(len);
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], len);
		in.close();
		return (contents);
	}
	auto code = errc::make_error_code(errc::no_such_file_or_directory);
	throw fs::filesystem_error("file not found", code);
}

std::unique_ptr<char[]>
RC2::ReadFileBlob(std::string filePath, size_t &size)
{
	using namespace std;
	fstream file(filePath, ios::in|ios::binary|ios::ate);
	size = file.tellg();
	unique_ptr<char[]> buffer(new char[size]);
	file.seekg(0, ios::beg);
	file.read(buffer.get(), size);
	file.close();
	return buffer;
}


std::string
RC2::GenerateUUID()
{
	uuid_t uuid;
	char uuidStr[40];
	uuid_generate(uuid);
	uuid_unparse_lower(uuid, uuidStr);
	return uuidStr;
}

std::string
RC2::GetPathForExecutable(pid_t pid)
{
#if defined(__APPLE__)
	/* for linux, readlink("/proc/self/exe", buf, bufsize)
		http://stackoverflow.com/questions/933850/how-to-find-the-location-of-the-executable-in-c
	*/
	char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
	int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
	if (ret > 0)
		return pathbuf;
#else
	char buff[1024];
	size_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
	if (len < 0) {
		throw std::runtime_error("failed to get exe path via readlink:%d" + errno);		
	}
	buff[len] = 0;
	return std::string(buff);
#endif
	return "";
}

int 
RC2::MakeDirectoryPath(std::string s, mode_t mode)
{
	size_t pre=0,pos=0;
	std::string dir;
	int mdretval=0;
	
	//make sure ends with a trailing slash
	if (s[s.size()-1] != '/') {
		s += '/';
	}
	
	while ((pos = s.find_first_of('/', pre)) != std::string::npos) {
		dir = s.substr(0, pos++);
		pre = pos;
		if (dir.size() == 0)
			continue; //skip leading /
		if (boost::filesystem::exists(dir) && boost::filesystem::is_directory(dir))
			continue;
		if ((mdretval = mkdir(dir.c_str(), mode)) && errno != EEXIST) {
			return mdretval;
		}
	}
	return mdretval;
}

std::string
RC2::PrivatePackagePath()
{
	std::string installLoc = RC2::GetPathForExecutable(getpid());
	std::string::size_type pos = installLoc.rfind('/');
	
	//add our package directory to the R library search path
	std::string pkgPath = installLoc.substr(0, pos) + "/pkgs/";
	if (!boost::filesystem::is_directory(pkgPath)) {
		boost::filesystem::path localpath(installLoc);
		localpath = localpath.parent_path() /= "R";
		if (boost::filesystem::is_directory(localpath))
			pkgPath = localpath.c_str();
	}
	return pkgPath;
}
