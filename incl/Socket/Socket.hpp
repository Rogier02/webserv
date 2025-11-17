#ifndef SOCKET_HPP
# define SOCKET_HPP

// C
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
// C++
# include <stdexcept>
// webserv
# include "EasyThrow.hpp"

class	Socket
{
	public:
		Socket();
		virtual ~Socket();

	public:
		operator int() const;

	public:
		int			accept() const;

		static int			accept(int fd);
		static std::string	recv(int fd);
	
	// Rogier's additions Nov 16 - start
		static int			send(int fd, const std::string& data);
	// End

	protected:
		int	_fd;
};

#endif
