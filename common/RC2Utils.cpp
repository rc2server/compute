#include "RC2Utils.hpp"
#include <unistd.h>
#include <uuid/uuid.h>
#if defined(__APPLE__)
#include <libproc.h>
#else
typedef char* uuid_string_t;
#endif
#include <sys/stat.h>
#include <cerrno>
#include <fstream>
#include <boost/filesystem.hpp>
#include "json/reader.h"
#include "json/writer.h"

RC2::TemporaryDirectory::TemporaryDirectory()
{
	_eraseOnDeath = true;
	std::string uuid = GenerateUUID();
	_path = "/tmp/" + uuid;
	if (mkdir(_path.c_str(), 0700) != 0) {
		throw std::runtime_error("failed to create directory:" + _path);
	}
}

RC2::TemporaryDirectory::TemporaryDirectory(std::string path, bool erase)
	: _path(path), _eraseOnDeath(erase)
{
	if (RC2::MakeDirectoryPath(_path, 0700) != 0) {
		throw std::runtime_error("failed to create directory:" + _path);
	}
}

RC2::TemporaryDirectory::~TemporaryDirectory()
{
	if (_eraseOnDeath)
		boost::filesystem::remove_all(_path);
}

RC2::JsonDictionary::JsonDictionary()
{
}

RC2::JsonDictionary::~JsonDictionary()
{
}

void
RC2::JsonDictionary::addBool(std::string key, bool val)
{
	_doc[key] = json::Boolean(val);
}

void
RC2::JsonDictionary::addInt(std::string key, int value)
{
	_doc[key] = json::Number(value);
}

void
RC2::JsonDictionary::addString(std::string key, std::string value)
{
	_doc[key] = json::String(value);
}

void
RC2::JsonDictionary::addObject(std::string key, json::Object &value)
{
	_doc[key] = value;
}

//rapidjson is fucked up w/o a copy syntax. so the array added to doc is dealloc'd
// when this method returns.
std::string
RC2::JsonDictionary::addStringArray(std::string key, std::vector<std::string> strings)
{
	json::Array array;
	for (std::vector<std::string>::const_iterator i = strings.begin(); i != strings.end(); ++i) {
		array.Insert(json::String(*i));
	}
	_doc[key] = array;
	return string();
}

std::string
RC2::JsonDictionary::string() const
{
	std::stringstream stream;
	json::Writer::Write(_doc, stream, false);
	return stream.str();
}

RC2::JsonDictionary::operator std::string () const
{
	return string();
}

std::string
RC2::SlurpFile(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw(errno);
}

std::runtime_error 
RC2::FormatErrorAsJson(int errorCode, std::string details)
{
	RC2::JsonDictionary json;
	json.addString("msg", "error");
	json.addInt("errorCode", errorCode);
	json.addString("errorDetails", details);
	return std::runtime_error(json);
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

