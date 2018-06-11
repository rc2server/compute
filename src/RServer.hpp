/* 
 * File:   RServer.hpp
 * Author: mlilback
 *
 * Created on March 2, 2015, 5:19 PM
 */

#ifndef RSERVER_HPP
#define	RSERVER_HPP

#include <event2/event.h>
#include <boost/noncopyable.hpp>

class RServer : private boost::noncopyable
{
public:
	RServer();
	virtual ~RServer();

	bool parseArgs(int argc, char **argv);
	void	startRunLoop();
	void	terminate();
	void handleEvent(evutil_socket_t listener, short events);

private:
	struct event_base*	_eventBase;
	bool				_verbose;
	bool				_shouldFork;
	uint				_port;
	int					_socket;
};

#endif	/* RSERVER_HPP */

