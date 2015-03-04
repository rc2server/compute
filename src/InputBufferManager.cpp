#include "InputBufferManager.hpp"

RC2::InputBufferManager::InputBufferManager()
{
}

RC2::InputBufferManager::~InputBufferManager()
{
}

void
RC2::InputBufferManager::appendData(struct evbuffer *inBuffer)
{
}

bool
RC2::InputBufferManager::hasCompleteMessage()
{
	return false;
}

std::string
RC2::InputBufferManager::popCurrentMessage()
{
	return "";
}
