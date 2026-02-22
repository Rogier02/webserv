#include "Event.hpp"

Event::Event(int fd, u_int32_t eventTypes)
	:	_signal(Signal::OK)
{
	data.fd = fd;
	events = eventTypes;
}

Event::Signal	Event::handle()
{
	_signal = Signal::OK;

	if (events & Epoll::Events::In)
		_in();
	if (events & Epoll::Events::Out)
		_out();

	return (_signal);
}

void	Event::_in() {}

void	Event::_out() {}

