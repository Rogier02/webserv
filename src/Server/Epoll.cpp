#include "Epoll.hpp"

Epoll::Epoll(int socket)
	:	_fd(epoll_create(1))
{
	if (_fd == -1)
		perror("epoll_create()");

	epoll_event ev{};
	ev.events = EPOLLIN;
	ev.data.fd = socket;

	if (epoll_ctl(_fd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
		perror("epoll_ctl()");
}

Epoll::~Epoll()
{
	close(_fd);
}