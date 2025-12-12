#include "Event.hpp"

Event::Event(Epoll::Events eventTypes, int fd)
{
	events = eventTypes;
	data.fd = fd;
}

void	Event::handle()
{
	if (events & Epoll::Events::In)
		_in();
	if (events & Epoll::Events::Out)
		_out();
}

void	Event::_in() const {}

void	Event::_out() const {}

// should this give the user an 'internal err' error page?
Event::CloseConnection::CloseConnection(int fd)
	:	std::runtime_error("Connection error, needs closing")
	,	_fd(fd)
{}

int Event::CloseConnection::fd() {
	return (_fd);
}
