#include "Event.hpp"

Event::Event(EpollEvents eventTypes, int fd)
{
	events = eventTypes;
	data.fd = fd;
}

Event::operator int() const {
	return (data.fd);
}

void	Event::handle()
{
	if (events & EpollEvents::In)
		_in();
	if (events & EpollEvents::Out)
		_out();
}

void	Event::_in() const {}

void	Event::_out() const {}
