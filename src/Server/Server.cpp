#include "Server.hpp"

std::atomic<bool> Server::_running(false);

Server::Server()
	:	_socket(socket(AF_INET, SOCK_STREAM, 0))
	,	_port(8080)
	,	_epoll(_socket)
{
	if (_socket == -1)
		throw std::runtime_error("socket()");

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt(SO_REUSEADDR)");

	sockaddr_in	serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(_port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("bind()");

	if (listen(_socket, 5) == -1)
		throw std::runtime_error("listen()");

	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown);
}

Server::~Server()
{
	close(_socket);
}

void
Server::run()
{
	_running = true;
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_running)
	{
		epoll_event events[EventBatchSize];
		int nEvents = epoll_wait(_epoll.fd(), events, EventBatchSize, 1000);
		if (nEvents == -1) {
			perror("epoll_wait");
			continue;
		}

		for (int i = 0; i < nEvents; ++i) {
			if (events[i].events & (EPOLLERR | EPOLLHUP))
			{
				close(events[i].data.fd);
				epoll_ctl(_epoll.fd(), EPOLL_CTL_DEL, events[i].data.fd, nullptr);
				continue;
			} else
			if (events[i].data.fd == _socket)
			{
				int clientSocket = accept(_socket, nullptr, nullptr);
				if (clientSocket == -1) {
					perror("accept");
					continue;
				}
				std::cout << "New client connected.\n";

				epoll_event clientEv{};
				clientEv.events = EPOLLIN;
				clientEv.data.fd = clientSocket;
				epoll_ctl(_epoll.fd(), EPOLL_CTL_ADD, clientSocket, &clientEv);
			} else
			{
				int clientFd = events[i].data.fd;
				char buffer[1024];
				ssize_t nBytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
				if (nBytes <= 0) {
					std::cout << "Client disconnected.\n";
					close(clientFd);
					epoll_ctl(_epoll.fd(), EPOLL_CTL_DEL, clientFd, nullptr);
				} else {
					buffer[nBytes] = '\0';
					std::cout << "Message from client: " << buffer << std::endl;
				}
			}
		}
	}
	std::cout << "Server shutting down...\n";
}

void
Server::shutdown(int) {
	_running = false;
	std::cerr << std::endl;
}
