#include <iostream>
#include <string>
#include <memory>
#include "InputBufferManager.hpp"

using namespace std;

const uint32_t kRSessionMagicNumber = 0x21;


RC2::InputBufferManager::InputBufferManager()
{
	_buffer = evbuffer_new();
}

RC2::InputBufferManager::~InputBufferManager()
{
	evbuffer_free(_buffer);
}
 
void
RC2::InputBufferManager::appendData(struct evbuffer *inBuffer)
{
	if (evbuffer_add_buffer(_buffer, inBuffer) == -1) {
		cerr << "failed to append buffer" << endl;
	}
	size_t dataSize = evbuffer_get_length(_buffer);
	if (dataSize > 8) {
		char dataHead[8];
		evbuffer_copyout(_buffer, dataHead, 8);
		uint32_t magicIdent = ntohl(*((uint32_t*)&dataHead[0]));
		uint32_t jsonSize = ntohl(*((uint32_t*)&dataHead[4]));
		if (magicIdent != kRSessionMagicNumber) {
			cerr << "bad magic number" << endl;
			this->reset();
			return;
		}
		if ((jsonSize + 8) <= dataSize) {
			std::unique_ptr<char[]> buff(new char[dataSize]);
			evbuffer_remove(_buffer, buff.get(), dataSize);
			_currentJson = string(buff.get()[8], jsonSize);
		}
	}
}

bool
RC2::InputBufferManager::hasCompleteMessage()
{
	return _currentJson.length() > 0;
}

std::string
RC2::InputBufferManager::popCurrentMessage()
{
	string str(_currentJson);
	_currentJson = "";
	return str;
}

void
RC2::InputBufferManager::reset()
{
	size_t dataSize = evbuffer_get_length(_buffer);
	evbuffer_drain(_buffer, dataSize);
	_expectedSize = 0;
	_currentJson = "";
}
