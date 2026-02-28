#include "Socket.hpp"

namespace	Socket
{
	// move this into ListenEvent constructor if possible
	int
	create(int port)
	{
		int	fd = EasyThrow(socket(AddressFamily::Inet, SocketType::Stream, 0));

		int opt = 1;
		EasyThrow(setsockopt(fd, SocketOptionLevel::Socket, SocketOption::ReuseAddress, &opt, sizeof(opt)));

		InternetSocketAddress	listenerAddress;
		listenerAddress.family	= AddressFamily::Inet;
		listenerAddress.port	= htons(port);
		listenerAddress.address	= InternetAddress::Any;

		EasyThrow(bind(fd, (SocketAddress *)&listenerAddress, sizeof(listenerAddress)));
		EasyThrow(listen(fd, 5));

		return (fd);
	}

	int
	accept(int fd)
	{
		return (EasyThrow(::accept(fd, nullptr, nullptr)));
	}

	ssize_t
	recv(int fd, std::string &dest)
	{
		char	buffer[1024];

		while (true) {
			ssize_t	received = ::recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);

			if (received > 0)
				dest.append(buffer, received);

			if (received == 0)
				return (0);

			if (received == -1) {
				if (errno == EAGAIN)
					return (-1);
				else return (0);
			}
		}
	}

	ssize_t
	send(int fd, const std::string& data)
	{
		return(EasyThrow(::send(fd, data.c_str(), data.length(), 0)));
	}
}
