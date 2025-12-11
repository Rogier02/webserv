#include "Socket.hpp"

Socket::Socket()
{}

Socket::~Socket() {
	close(_fd);
}

Socket::operator int() const {
	return (_fd);
}

int
Socket::accept(int fd)
{
	return (EasyThrow(::accept(fd, nullptr, nullptr)));
}

std::string
Socket::recv(int fd){
	char	buffer[1024];

	ssize_t nBytes = EasyThrow(::recv(fd, buffer, sizeof(buffer) - 1, 0));
	return (std::string(buffer, nBytes));
}

int
Socket::send(int fd, const std::string& data)
{
	return(EasyThrow(::send(fd, data.c_str(), data.length(), 0)));
}
