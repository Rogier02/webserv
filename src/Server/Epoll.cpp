#include "Epoll.hpp"

Epoll::Epoll(int socket)
	:	_fd(epoll_create(1))
{
	if (_fd == -1)
		throw std::runtime_error("epoll_create()");

	epoll_event	ev{};
	ev.events	= EPOLLIN;
	ev.data.fd	= socket;

	if (epoll_ctl(_fd, Ctl::Add, ev.data.fd, &ev) == -1)
		throw std::runtime_error("epoll_ctl()");
}

Epoll::~Epoll() {
	close(_fd);
}

Epoll::operator int() const {
	return (_fd);
}

std::vector<epoll_event> &
Epoll::wait()
{
	epoll_event buffer[_EventBatchSize];

	int nEvents = epoll_wait(_fd, buffer, _EventBatchSize, _waitTimeout);
	if (nEvents == -1) {
		perror("epoll_wait");
	} else for (int i = 0; i < nEvents; ++i) {
		_events.push_back(std::move(buffer[i]));
	}
	return (_events);
}

int
Epoll::ctl(Epoll::Ctl operation, int fd, epoll_event *event)
const {
	return (epoll_ctl(_fd, operation, fd, event));
}
