#ifndef EPOLL_HPP
# define EPOLL_HPP
# ifdef VERBOSE
#  include <iostream>
# endif

#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

class	Epoll
{
	public:
		Epoll(int socket);
		~Epoll();

	private:
		int	_fd;
};

#endif