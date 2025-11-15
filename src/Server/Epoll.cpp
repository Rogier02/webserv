#include "Epoll.hpp"

Epoll::Epoll()
	:	_fd(create())
{}

Epoll::~Epoll() {
	close(_fd);
}

Epoll::operator int() const {
	return (_fd);
}

int	Epoll::create() const {
	return (EasyThrow(epoll_create(1)));
}

std::vector<epoll_event>
Epoll::wait()
const {
	epoll_event	buffer[_EventBatchSize];

	int nEvents = EasyThrow(epoll_wait(_fd, buffer, _EventBatchSize, _waitTimeout));
	return (std::vector<epoll_event>(buffer, (epoll_event *)(buffer + nEvents)));
}

int	Epoll::ctl(Epoll::Ctl operation, int fd, epoll_event *event) const {
	return (EasyThrow(epoll_ctl(_fd, operation, fd, event)));
}

int	Epoll::ctl(Epoll::Ctl operation, epoll_event &event) const {
	return (ctl(operation, event.data.fd, &event));
}
