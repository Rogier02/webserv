#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "Epoll.hpp"

class	Server
{
	public:
		Server();
		~Server();

		void	run();

	private:
		int		_socket;
		Epoll	_epoll;

};

#endif