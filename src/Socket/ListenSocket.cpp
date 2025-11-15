#include "ListenSocket.hpp"

ListenSocket::ListenSocket(int port)
{
	_fd = EasyThrow(socket(AddressFamily::Inet, SocketType::Stream, 0));

	int opt = 1;
	EasyThrow(setsockopt(_fd, SocketOptionLevel::Socket, SocketOption::ReuseAddress, &opt, sizeof(opt)));

	InternetSocketAddress	serverAddress;
	serverAddress.family	= AddressFamily::Inet;
	serverAddress.port		= htons(port);
	serverAddress.address	= InternetAddress::Any;

	EasyThrow(bind(_fd, (SocketAddress *)&serverAddress, sizeof(serverAddress)));
	EasyThrow(listen(_fd, 5));
}
