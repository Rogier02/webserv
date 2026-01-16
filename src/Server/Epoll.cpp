#include "Epoll.hpp"

Epoll::Epoll()
	:	_epfd(EasyThrow(epoll_create(1)))
{
	rlimit	resourceLimit;
	EasyThrow(getrlimit(RLIMIT_NOFILE, &resourceLimit));
	if (resourceLimit.rlim_cur == RLIM_INFINITY)
		throw LocatedThrow("Epoll resource limit (nofile) is too large");
	_buffer = new epoll_event[resourceLimit.rlim_cur];

	EasyPrint(_epfd);
	EasyPrint(resourceLimit.rlim_cur);
}

Epoll::~Epoll()
{
	close(_epfd);
	delete[] _buffer;
}

std::vector<epoll_event>
Epoll::wait(int timeout_ms)
{
	int nEvents = EasyThrow(epoll_wait(_epfd, _buffer, sizeof(_buffer), timeout_ms));
	return (std::vector<epoll_event>(_buffer, (epoll_event *)(_buffer + nEvents)));
}

int	Epoll::ctl(Epoll::Ctl operation, int fd, epoll_event *event) {
	return (epoll_ctl(_epfd, operation, fd, event));
}

int	Epoll::ctl(Epoll::Ctl operation, epoll_event &event) {
	return (ctl(operation, event.data.fd, &event));
}
