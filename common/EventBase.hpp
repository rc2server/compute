#pragma once

#include <event2/event.h>
#include <memory>
#include <exception>
#include <iostream>
#include <string>
#include <set>
#include "Event.hpp"

namespace RC2 {

class EventBase {
	friend Event;
public:
	///default constructor
	EventBase();
	///takes ownership of argument
	EventBase(event_base*);
	///move constructor
	EventBase(EventBase&&);
	///destructor
	virtual ~EventBase();
	///move operator
	EventBase& operator=(EventBase&&);
	
	event_base* base() const noexcept { return _base; };
	operator event_base*() const noexcept { return _base; }
	
	int eventCount() const { return _events.size(); }
	///takes ownership of event
	void addEvent(std::shared_ptr<Event> event, long milliseconds = 0);
	void removeEvent (std::shared_ptr<Event> event);
	
	///defaults to a null stream. will never return nullptr
	static const std::ostream* logStream() { return _logStream; }
	///if ostream == nullptr, sets it to a nullstream so logStream() will never return nullptr
	static void setLogger(std::ostream *ostream);
	
protected:
	void logMessage(std::string message);
	
	std::set< std::weak_ptr<Event>, std::owner_less< std::weak_ptr<Event> > > _events;
	event_base* _base;
	static std::ostream* _logStream;
};	
	
};


struct EventException: public std::runtime_error {
	EventException(std::string const& message)
	: std::runtime_error(message)
	{}
};
