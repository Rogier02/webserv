// C++ program to illustrate the client application in the
// socket programming

#include <errno.h>
#include <stdio.h>
#include <exception>
#include <iostream>
#include <ifstream>


int main(int argc, char **argv)
{
	if (argc == 2)
	{

	}
	try {
		Server	server(config._servers);
		server.run();
	} catch (std::exception &exception) {
		perror(exception.what());
	}
	return (0);
}
