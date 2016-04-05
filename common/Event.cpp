#include "Event.hpp"
#include "EventBase.hpp"

RC2::Event::Event(std::shared_ptr<EventBase>& base, int fd, short int what, EventCallback handler)
	:_base(base), _handler(std::move(handler)), _description("<event>")
{
	auto strongBase = _base.lock();
	if (nullptr == strongBase) {
		throw EventException("invalid EventBase");
	}
	_event = event_new(strongBase->base(), fd, what, RC2::Event::handleCallback, this);
}

RC2::Event::Event (RC2::Event&& rhs)
	: _event(rhs._event), _base(rhs._base), _handler(rhs._handler)
{
	rhs._event = nullptr;
}


RC2::Event::~Event()
{
	event_free(_event);
}

void RC2::Event::handleCallback (int fd, short int what, void* arg)
{
	Event* event = reinterpret_cast<Event*>(arg);
	std::string msg = "event " + event->description() + " fired";
	event->logMessage(msg);
	event->_handler(fd, what, event);
}

void RC2::Event::logMessage ( std::string message ) const
{
	auto strongBase = _base.lock();
	if (nullptr != strongBase)
		strongBase->logMessage(message);
}

