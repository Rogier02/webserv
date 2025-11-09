// C++ program to illustrate the client application in the
// socket programming

#include "Config.hpp"
#include <stdio.h>
#include <exception>
#include <iostream>
#include <ifstream>


int main(int argc, char **argv)
{
	if (argc == 2)
	{
		const char *fileName = argv[1];
		ReadConfigFile(fileName);
	}
	try {
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		perror(exception.what());
	}
	return (0);
}
