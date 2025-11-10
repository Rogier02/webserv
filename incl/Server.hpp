#ifndef SERVER_HPP
# define SERVER_HPP

// C
# include <iostream>
# include <netinet/in.h>
# include <signal.h>
// C++
# include <stdexcept>
# include <atomic>

# include "Config.hpp"
# include "Socket.hpp"
# include "Epoll.hpp"

class	Server
{
	private:
		static constexpr int		_EventBatchSize = 64;
		static constexpr int		_DefaultPort = 8080;

		static std::atomic<bool>	_running;

	public:
		Server(std::vector<Config::Server> const &config);
		Server(Server const &other) = delete;
		Server &operator=(Server const &other) = delete;
		~Server() = default;

	private:
		std::vector<Config::Server> const &_config;
		Socket	_socket;
		Epoll	_epoll;
		int		_port = _DefaultPort;
	
	public:
		void		run() const;
		static void	shutdown(int);

	private:
		void	newClient() const;
		void	existingClient(int fd) const;
		void	zombieClient(int fd) const;

};

#endif
