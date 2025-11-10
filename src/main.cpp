// C++ program to illustrate the client application in the
// socket programming

#include <errno.h>
#include <stdio.h>
#include <exception>

#include "Server.hpp"
#include "Config.hpp"
#include <stdexcept>

int	main(int argc, char **argv)
{
	if (argc > 2) {
		std::cout << "Usage: " << argv[0] << " [configuration file]\n";
		return (EINVAL);
	}

	Config config;
	if (argc == 2)
		config = Config(argv[2]);

	try {
		Server	server(config._servers);
		server.run();
	} catch (std::exception &exception) {
		perror(exception.what());
	}
	return (0);
}
