#include "Server.hpp"

std::atomic<bool> Server::_running(false);

Server::Server()
	:	_socket(_port)
	,	_epoll(_socket)
{
	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown); // should this happen in main() or ::run() instead? static _running doesn't work for multiple servers anyway
}

/* Server::Server(std::vector<Config::Server> const &config)
	:	_config(config)
	,	_socket()
	,	_epoll(_socket)
{
	(void)_config;

	sockaddr_in	serverAddress{};
	serverAddress.sin_family		= AF_INET;
	serverAddress.sin_port			= htons(_port);
	serverAddress.sin_addr.s_addr	= INADDR_ANY;

	if (bind(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("bind()");

	if (listen(_socket, 5) == -1)
		throw std::runtime_error("listen()");

	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown);
} */

void
Server::run()
const {
	_running = true;
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_running)
	{
		try {
			for (Epoll::Event &event : _epoll.wait())
			{
				if (event.isWeird())
					zombieClient(event.fd);
				else if (event.fd == _socket)
					newClient();
				else
					existingClient(event.fd);// this should absolutely pass a Socket... but how do I make Event::Fd a Socket???
			}
		} catch	(std::runtime_error &restartRequired) {
			// log restart
			std::cerr << restartRequired.what() << std::endl;
		} catch (std::exception &unknownException) {
			// log unknown and throw to main? or just don't catch and let Logger class catch somehow?
			throw unknownException;
		}
	}
	std::cout << "Server shutting down...\n";
}

void
Server::shutdown(int)
{
	_running = false;
	std::cerr << std::endl;
}

void
Server::newClient()
const {
	Epoll::Event	event(_socket.accept());

	_epoll.ctl(Epoll::Ctl::Add, event);
	std::cout << "New client connected.\n";
}

void
Server::existingClient(int fd)
const {
	std::string	request = Socket::recv(fd);
	if (request.empty()) {
		std::cout << "Client disconnected.\n";
		_epoll.ctl(Epoll::Ctl::Del, fd);
		close(fd);
	} else {
		std::cout << "Client Request:\n" << request << std::endl;
	}
}

void
Server::zombieClient(int fd)
const {
	close(fd);
	_epoll.ctl(Epoll::Ctl::Del, fd);
}
