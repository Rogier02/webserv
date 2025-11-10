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
	static constexpr int		EventBatchSize = 64;
	static constexpr int		DefaultPort = 8080;

	public:
		Server(std::vector<Config::Server> const &config);
		Server(Server const &other) = delete;
		Server &operator=(Server const &other) = delete;
		~Server() = default;

		void		run() const;
		static void	shutdown(int);

	private:
		static std::atomic<bool>	_running;

		// class-wide variables
		std::vector<Config::Server> const &_config;
		Socket	_socket;
		Epoll	_epoll;
		int		_port = DefaultPort;

		// epoll_event handlers
		void	newClient() const;
		void	existingClient(int fd) const;
		void	zombieClient(int fd) const;
};

#endif
