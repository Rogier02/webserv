#include "Server.hpp"

std::atomic<bool> Server::_running(false);

// note: _socket() default constructor is implicit, call can be removed from initialiser list
Server::Server(std::vector<Config::Server> const &config)
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
}

void
Server::run()
const {
	_running = true;
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_running)
	{
		epoll_event buffer[_EventBatchSize];

		int nEvents = epoll_wait(_epoll, buffer, _EventBatchSize, 1000);
		if (nEvents == -1) {
			perror("epoll_wait");
			continue;
		}

		for (int i = 0; i < nEvents; ++i)
		{
			epoll_event	&event = buffer[i];

			if (event.events & (EPOLLERR | EPOLLHUP))
				zombieClient(event.data.fd);
			else
			if (event.data.fd == _socket)
				newClient();
			else
				existingClient(event.data.fd);
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
	int clientSocket = accept(_socket, nullptr, nullptr);
	if (clientSocket == -1) {
		perror("accept");
		return;
	}

	std::cout << "New client connected.\n";

	epoll_event clientEv{};
	clientEv.events = EPOLLIN;
	clientEv.data.fd = clientSocket;
	epoll_ctl(_epoll, EPOLL_CTL_ADD, clientSocket, &clientEv);
}

void
Server::existingClient(int fd)
const {
	char buffer[1024];
	ssize_t nBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (nBytes <= 0) {
		std::cout << "Client disconnected.\n";
		close(fd);
		epoll_ctl(_epoll, EPOLL_CTL_DEL, fd, nullptr);
	} else {
		buffer[nBytes] = '\0';
		std::cout << "Message from client: " << buffer << std::endl;
	}
}

void
Server::zombieClient(int fd)
const {
	close(fd);
	epoll_ctl(_epoll, EPOLL_CTL_DEL, fd, nullptr);
}
