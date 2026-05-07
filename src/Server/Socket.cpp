#include "Socket.hpp"

namespace	Socket
{
	int
	create(std::string host, int port)
	{
		int	fd = -1;
		int	opt	= 1;

		addrinfo	*aip;

		try {
			addrinfo			hints{};
			hints.ai_flags		= AI_PASSIVE;
			hints.ai_family		= AF_INET;
			hints.ai_socktype	= SOCK_STREAM;

			EasyThrow(getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &aip));
			fd = EasyThrow(::socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol));
			EasyThrow(::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)));
			EasyThrow(::bind(fd, aip->ai_addr, aip->ai_addrlen));
			EasyThrow(::listen(fd, 10));

			freeaddrinfo(aip);
		} catch (std::exception const &e) {
			if (fd != -1)
				::close(fd);
			freeaddrinfo(aip);
			throw;
		}

		return (fd);
	}

	int
	accept(int fd)
	{
		return (EasyThrow(::accept(fd, nullptr, nullptr)));
	}

	::ssize_t
	recv(int fd, std::string &dest)
	{
		char	buffer[bufferSize];

		::ssize_t	received = EasyThrow(::recv(fd, buffer, sizeof(buffer), MSG_DONTWAIT));

		if (received > 0)
			dest.append(buffer, received);

		LOG(Debug, "Received " + std::to_string(received) + " bytes from client " + std::to_string(fd));
		return (received);
	}

	::ssize_t
	send(int fd, const std::string &data)
	{
		return(EasyThrow(::send(fd, data.c_str(), data.length(), 0)));
	}
}
