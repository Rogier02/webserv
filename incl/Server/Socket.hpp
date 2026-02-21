#ifndef SOCKET_HPP
# define SOCKET_HPP

// C
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
// webserv
# include "EasyThrow.hpp"

# include "WrapIn.hpp"

using	SocketType				= WrapIn::SocketType;
using	SocketOptionLevel		= WrapIn::SocketOptionLevel;
using	SocketOption			= WrapIn::SocketOption;
using	SocketAddress			= WrapIn::SocketAddress;
using	InternetAddress			= WrapIn::InternetAddress;
using	AddressFamily			= WrapIn::AddressFamily;
using	InternetSocketAddress	= WrapIn::InternetSocketAddress;

namespace	Socket
{
	int			create(int port);
	int			accept(int fd);
	ssize_t		recv(int fd, std::string &dest);
	ssize_t		send(int fd, std::string const &data);
};

#endif
