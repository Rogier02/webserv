#ifndef SOCKET_HPP
# define SOCKET_HPP

// C
# include <unistd.h>
# include <netdb.h>
# include <sys/socket.h>
// webserv
# include "EasyThrow.hpp"
# include "Logger.hpp"

namespace	Socket
{
	struct	InternetSocketAddress : private sockaddr_in {
		private:
			using	Family	= sa_family_t;
			using	Port	= in_port_t;
			using	Address	= in_addr_t;
		public:
			Family	&family	= sockaddr_in::sin_family;
			Port	&port	= sockaddr_in::sin_port;
			Address	&address= sockaddr_in::sin_addr.s_addr;
	};

	constexpr ::size_t	bufferSize = 1024;

	int			create(std::string host,int port);
	int			accept(int fd);
	::ssize_t	recv(int fd, std::string &dest);
	::ssize_t	send(int fd, std::string const &data);
};

#endif
