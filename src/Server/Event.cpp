#include "Event.hpp"

EpollEvent::EpollEvent(int socket)
{
	this->events = EPOLLIN;
	setFd(socket);
}

EpollEvent::operator int() const
{
	return(getFd());
}

int
EpollEvent::getFd() const
{
	return (epoll_event::data.fd);
}

void
EpollEvent::setFd(int fd_value)
{
	epoll_event::data.fd = fd_value;
}

bool
EpollEvent::isWeird() const
{
	return (events & (EPOLLERR | EPOLLHUP));
}