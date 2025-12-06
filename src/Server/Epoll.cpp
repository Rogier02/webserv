#include "Epoll.hpp"

Epoll::Epoll()
	:	_fd(create())
{}

Epoll::Epoll(ListenSocket const &listenSocket)
	:	_fd(create())
{
	Event	event(listenSocket);
	ctl(Ctl::Add, event.data.fd, &event);
}

Epoll::~Epoll() {
	close(_fd);
}

Epoll::operator int() const {
	return (_fd);
}

int	Epoll::create() const {
	return (EasyThrow(epoll_create(1)));
}

std::vector<Epoll::Event>
Epoll::wait()
const {
	Event	buffer[_EventBatchSize];

	int nEvents = EasyThrow(epoll_wait(_fd, buffer, _EventBatchSize, _waitTimeout));
	return (std::vector<Event>(buffer, (Event *)(buffer + nEvents)));
}

int	Epoll::ctl(Epoll::Ctl operation, int fd, Event *event) const {
	return (EasyThrow(epoll_ctl(_fd, operation, fd, event)));
}

int	Epoll::ctl(Epoll::Ctl operation, Event &event) const {
	return (ctl(operation, event.data.fd, &event));
}

