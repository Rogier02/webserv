#ifndef SERVER_HPP
# define SERVER_HPP

// C
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
# include "ListenSocket.hpp"
# include "Epoll.hpp"
# include "EventTypes.hpp"
# include "EasyThrow.hpp"
# include "EasyPrint.hpp"
# include "Logger.hpp"
// events
# include "ClientEvent.hpp"
# include "ListenEvent.hpp"

extern	std::atomic<bool>	_pleaseShutDown;

class	Server //Should be main biggest webserv class thingie
{
	public:
		Server() = delete;
		Server(Server const &) = delete;
		Server(Server &&) = delete;
		Server(Config &config);
		~Server() = default;

	private:
		std::vector<Server>			servers;
		Epoll						_epoll;
		std::vector<ListenSocket>	_listenSockets;//should be in Server(which is now still Config)

	public:
		void	run();

	private:
		void	_closeConnection(int fd);
};

#endif
