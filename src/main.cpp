// C++ program to illustrate the client application in the
// socket programming

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <csignal>
#include <atomic>

#include "Server.hpp"

std::atomic<bool>	running(true);

constexpr int	eventBatchSize	= 64;

int main()
{
	Server	server;
	std::cout << "Server listening on port 8080...\n";

	while (running) {
		epoll_event events[eventBatchSize];
		int nEvents = epoll_wait(epollFd, events, eventBatchSize, 1000);
		if (nEvents == -1) {
			perror("epoll_wait");
			continue;
		}

		for (int i = 0; i < nEvents; ++i) {
			if (events[i].data.fd == serverSocket) {
				int clientSocket = accept(serverSocket, nullptr, nullptr);
				if (clientSocket == -1) {
					perror("accept");
					continue;
				}
				std::cout << "New client connected.\n";

				epoll_event clientEv{};
				clientEv.events = EPOLLIN;
				clientEv.data.fd = clientSocket;
				epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &clientEv);
			} else {
				int clientFd = events[i].data.fd;
				char buffer[1024];
				ssize_t nBytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
				if (nBytes <= 0) {
					std::cout << "Client disconnected.\n";
					close(clientFd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, nullptr);
				} else {
					buffer[nBytes] = '\0';
					std::cout << "Message from client: " << buffer << std::endl;
				}
			}
		}
	}

	return 0;
}
