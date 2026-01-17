#ifndef SERVER_HPP
# define SERVER_HPP

// C
# include <signal.h>

// C++
# include <stdbool.h>
# include <iostream>
# include <stdexcept>
# include <atomic>

// webserv
# include "Config.hpp"
# include "ListenSocket.hpp"
# include "Epoll.hpp"
# include "Logger.hpp"
# include "ClientState.hpp"

class	Server
{
	private:
		static constexpr int	_DefaultPort = 8080;
		static std::atomic<bool>	_running;

	public:
		Server();
		Server(Server const &other) = delete;
		Server &operator=(Server const &other) = delete;
		~Server() = default;

	private:
		// std::vector<Config::Server> const &_config;
		int				_port = _DefaultPort;
		ListenSocket	_socket;
		Epoll			_epoll;
		CgiHandler		_cgiHandler;
		mutable			std::map<int, ClientState>	_clients;

		// Map CGI pipe FDs client FDs (for reading CGI output)
		mutable 		std::map<int, int> _cgiPipeToClientFd;

	public:
		void		run() const;
		static void	shutdown(int);

	private:
		void	newClient() const;
		void	existingClient(int fd) const;
		void	zombieClient(int fd) const;

		// State machine steps
		void	readRequest(int fd. ClientState& client) const;
		void	parseRequest(int fd, ClientState& client) const;
		void	generateResponse(int fd, ClientState& client) const;
		void	executeCgi(int fd, ClientState& client) const;
		void	readCgiOutput(int cgiPipefd) const;
		void	sendResponse(int fd, ClientState& client) const;
		void	cleanupClient(int fd, ClientState& client) const;
};

#endif
