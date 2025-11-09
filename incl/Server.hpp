#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdexcept>

#include <atomic>

#include "Epoll.hpp"

class	Server
{
	public:
		Server();
		~Server();

		void		run();
		static void	shutdown(int);

	private:
		int		_socket;
		Epoll	_epoll;

		static std::atomic<bool>	_running;
		static constexpr int		EventBatchSize = 64;
};

#endif
