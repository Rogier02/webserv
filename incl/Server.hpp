#ifndef SERVER_HPP
# define SERVER_HPP

// C
# include <signal.h>
// C++
# include <iostream>
# include <stdexcept>
# include <atomic>
// webserv
# include "Config.hpp"
# include "ListenSocket.hpp"
# include "Epoll.hpp"
# include "Logger.hpp"

extern	std::atomic<bool>	_running;

class	Server
{
	private:
		static constexpr int	_DefaultPort = 8080;

	public:
		Server();
		// Server(std::vector<Config::Server> const &config);
		Server(Server const &other) = delete;
		Server &operator=(Server const &other) = delete;
		~Server() = default;

	private:
		// std::vector<Config::Server> const &_config;
		int				_port = _DefaultPort;
		ListenSocket	_socket;
		Epoll			_epoll;

	public:
		void	run();

	private:
		void	addClient();
		void	delClient(int fd);
};

#endif
