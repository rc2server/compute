/* 
 * File:   RServer.hpp
 * Author: mlilback
 *
 * Created on March 2, 2015, 5:19 PM
 */

#ifndef RSERVER_HPP
#define	RSERVER_HPP

#include <event2/event.h>

class RServer
{
public:
	RServer();
	virtual ~RServer();

	bool parseArgs(int argc, char **argv);
	void	startRunLoop();

private:
	struct event_base*	_eventBase;
	bool				_verbose;
	uint				_port;
	int					_socket;
};

#endif	/* RSERVER_HPP */

