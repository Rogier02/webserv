#ifndef EPOLL_HPP
# define EPOLL_HPP

# ifdef VERBOSE
#  include <iostream>
# endif

// C
#include <unistd.h>
#include <sys/epoll.h>
// C++
#include <stdexcept>

class	Epoll
{
	public:
		Epoll(int socket);
		~Epoll();

		operator int() const;

	private:
		int	_fd;
};

#endif