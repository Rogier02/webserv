// C++ program to illustrate the client application in the
// socket programming

#include <stdio.h>
#include <exception>

#include "Server.hpp"

int main()
{
	try {
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		perror(exception.what());
	}
	return (0);
}
