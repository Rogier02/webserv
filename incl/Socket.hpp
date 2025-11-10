#ifndef SOCKET_HPP
# define SOCKET_HPP

# ifdef VERBOSE
#  include <iostream>
# endif

// C
# include <unistd.h>
# include <sys/socket.h>
// C++
# include <stdexcept>

class	Socket
{
	public:
		Socket();
		~Socket();

		operator int() const;

	private:
		int	_fd;
};

#endif
