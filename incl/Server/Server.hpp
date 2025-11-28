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
# include "Logger.hpp"
# include "ClientEvent.hpp"
// wrappers
# include "WrapEpoll.hpp"

extern	std::atomic<bool>	_pleaseShutDown;

class	Server
{
	public:
		using	EpollEvents	= enum WrapEpoll::Events;

	private:
		static constexpr int	_DefaultPort = 8080;

	public:
		Server();
		// Server(Config const &config);
		Server(Server const &other) = delete;
		Server &operator=(Server const &other) = delete;
		~Server() = default;

	private:
		int				_port = _DefaultPort;
		ListenSocket	_listenSocket;
		Epoll			_epoll;

	public:
		void	run();

	private:
		void	_addClient();
		void	_delClient(int fd);
};

#endif
