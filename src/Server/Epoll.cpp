#include "Epoll.hpp"

Epoll::Epoll()
	:	_epfd(EasyThrow(epoll_create(1)))
{
	rlimit	resourceLimit;
	EasyThrow(getrlimit(RLIMIT_NOFILE, &resourceLimit));
	if (resourceLimit.rlim_cur == RLIM_INFINITY)
		throw LocatedThrow("Epoll resource limit (nofile) is too large");
	_bufferSize = std::min((unsigned long)1024, resourceLimit.rlim_cur);

	EasyPrint(_epfd);
	EasyPrint(_bufferSize);
}

Epoll::~Epoll()
{
	close(_epfd);
}

std::vector<epoll_event>
Epoll::wait(int timeout_ms)
{
	epoll_event	buffer[_bufferSize];

	int nEvents = EasyThrow(epoll_wait(_epfd, buffer, _bufferSize, timeout_ms));
	return (std::vector<epoll_event>(buffer, (epoll_event *)(buffer + nEvents)));
}

int	Epoll::ctl(Epoll::Ctl operation, int fd, epoll_event *event) {
	return (epoll_ctl(_epfd, operation, fd, event));
}

int	Epoll::ctl(Epoll::Ctl operation, epoll_event &event) {
	return (ctl(operation, event.data.fd, &event));
}
