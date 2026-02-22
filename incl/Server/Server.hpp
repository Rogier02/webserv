#ifndef SERVER_HPP
# define SERVER_HPP

// C++
# include <stdbool.h>
# include <iostream>
# include <stdexcept>
# include <atomic>
# include <memory>
# include <vector>
# include <map>
// webserv
# include "Config.hpp"
# include "Epoll.hpp"
# include "EventHandlers.hpp"
# include "EasyThrow.hpp"
# include "EasyPrint.hpp"
# include "Logger.hpp"
// events
# include "ClientEvent.hpp"
# include "ListenEvent.hpp"

extern	std::atomic<bool>	_pleaseShutDown;

class	Server
{
	public:
		Server() = delete;
		Server(Server const &) = delete;
		Server(Server &&) = delete;
		Server(Config &config);
		~Server() = default;

	private:
		Epoll				_epoll;
		std::vector<int>	_listenSockets;

	public:
		void	run();

	private:
		void	_closeConnection(int fd);
		void	_readyToSend(int fd);
};

#endif
