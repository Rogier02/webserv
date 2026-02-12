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

// this class is weeeeeird

class	Socket
{
	public:
		Socket();
		Socket(Socket const &) = delete;
		Socket(Socket &&) = delete;
		virtual ~Socket();

	public:
		operator int() const;

	public:
		static int			accept(int fd);
		static std::string	recv(int fd);
		static int			send(int fd, std::string const &data);

	protected:
		int	_fd;
};

#endif
