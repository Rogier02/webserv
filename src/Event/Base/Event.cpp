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

Event::ShouldClose::ShouldClose(int fd)
	:	_fd(fd)
{}

int Event::ShouldClose::fd() {
	return (_fd);
}

const char *Event::ShouldClose::what() const throw() {
	return ("Event should close");
}