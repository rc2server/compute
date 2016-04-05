#include "EventBase.hpp"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

namespace bio = boost::iostreams;

static bio::stream< bio::null_sink > nullStream;
std::ostream* RC2::EventBase::_logStream = &nullStream;


RC2::EventBase::EventBase()
	: _events()
{
	struct event_config *config = event_config_new();
	event_config_require_features(config, EV_FEATURE_FDS);
	_base = event_base_new_with_config(config);
	event_config_free(config);
	//0=high, 1=normal, 2=low, 3=lowlow. default is num/2, so we need 4 to make default=1
	event_base_priority_init(_base, 4); 
}

RC2::EventBase::EventBase (event_base* eb)
	: _base(eb)
{

}

RC2::EventBase::~EventBase()
{
	if (_base)
		event_base_free(_base);
}

RC2::EventBase::EventBase ( RC2::EventBase&& that)
{
	_base = that._base;
	that._base = nullptr;
}

RC2::EventBase& RC2::EventBase::operator= ( RC2::EventBase&& that)
{
	_base = that._base;
	that._base = nullptr;
}

void RC2::EventBase::addEvent (std::shared_ptr<Event> event, long int milliseconds)
{
	struct timeval tv;
	struct timeval* tvPtr = &tv;
	if (0 == milliseconds) tvPtr = nullptr;
	event_add(event->_event, tvPtr);
	std::weak_ptr<Event> weakRef = event;
	_events.insert(weakRef);
	logMessage("added event " + event->description());
}

void RC2::EventBase::removeEvent (std::shared_ptr<Event> event)
{
	event_del(event->_event);
	_events.erase(event);
	logMessage("removed event " + event->description());
}

void RC2::EventBase::logMessage ( std::string message )
{
	(*EventBase::_logStream) << message << std::endl;
}


void RC2::EventBase::setLogger ( std::ostream* ostream )
{
	if (ostream)
		_logStream = ostream;
	else
		_logStream = &nullStream;
}
