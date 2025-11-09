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
#include <exception>

#include "Server.hpp"

std::atomic<bool>	_running(true);

int main()
{
	try {
		Server	server;
		server.run();
	} catch (std::exception &e) {
		perror(e.what());
	}
	return (0);
}
