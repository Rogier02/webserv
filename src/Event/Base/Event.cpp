#include "Event.hpp"

Event::Event(int fd, u_int32_t eventTypes)
{
	data.fd = fd;
	events = eventTypes;
}

void	Event::handle()
{
	if (events & Epoll::Events::In)
		_in();
	if (events & Epoll::Events::Out)
		_out();
}

void	Event::_in() {}

void	Event::_out() {}

Event::CloseConnection::CloseConnection(int fd)
	:	std::runtime_error("Connection error, needs closing")
	,	_fd(fd)
{}

int Event::CloseConnection::fd() {
	return (_fd);
}

Event::ReadyToSend::ReadyToSend(int fd)
	:	_fd(fd)
{}

int Event::ReadyToSend::fd() {
	return (_fd);
}
