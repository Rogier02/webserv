#include "Socket.hpp"

Socket::Socket()
	:	_fd(socket(AF_INET, SOCK_STREAM, 0))
{
	if (_fd == -1)
		throw std::runtime_error("socket()");

	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt(SO_REUSEADDR)");
}

Socket::~Socket() {
	close(_fd);
}

Socket::operator int() const {
	return (_fd);
}
