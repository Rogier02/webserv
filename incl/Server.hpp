#ifndef SERVER_HPP
# define SERVER_HPP

// C
# include <iostream>
# include <netinet/in.h>
# include <signal.h>
// C++
# include <stdexcept>
# include <atomic>

# include "Socket.hpp"
# include "Epoll.hpp"

class	Server
{
	public:
		Server();
		Server(Server const &other) = delete;
		Server &operator=(Server const &other) = delete;
		~Server() = default;

		void		run() const;
		static void	shutdown(int);

	private:
		static constexpr int		EventBatchSize = 64;
		static constexpr int		DefaultPort = 8080;

		static std::atomic<bool>	_running;

		// class-wide variables
		Socket	_socket;
		Epoll	_epoll;
		int		_port = DefaultPort;

		// epoll_event handlers
		void	newClient() const;
		void	existingClient(int fd) const;
		void	zombieClient(int fd) const;
};

#endif
