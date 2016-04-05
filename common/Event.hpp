#pragma once

#include <event2/event.h>
#include <memory>
#include <functional>

namespace RC2 {
	///forward declaration
	class EventBase;
	class Event;
	
	using EventCallback = std::function<void(int fd, short what, Event* event)>;

	///Wrapper around struct event from libevent
class Event {
public:
	friend EventBase;
	
	///designated constructor,calls new_event 
	Event(std::shared_ptr<EventBase>& base, int fd, short what, EventCallback handler);
	///move constructor
	Event(Event&& rhs);
	
	virtual ~Event();

	std::string description() const { return _description; }
	void setDescription(std::string desc) { _description = desc; }
	
protected:
	struct event*				_event;
	std::weak_ptr<EventBase> 	_base;
	EventCallback				_handler;
	std::string					_description;
	
	static void handleCallback(evutil_socket_t fd, short what, void *arg);
	void logMessage(std::string message) const;

private:
	Event() {};
};
	
};
