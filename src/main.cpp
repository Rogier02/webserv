#include <errno.h>
#include <stdio.h>
<<<<<<< HEAD
#include <exception>
#include <iostream>
#include <ifstream>

=======
#include <stdexcept>
#include <signal.h>

#include "Server.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"
>>>>>>> origin/rogier

int main(int argc, char **argv)
{
<<<<<<< HEAD
	if (argc == 2)
		config.loadFromFile(argv[2]);
=======
	if (argc > 2) {
		std::cout << "Usage: " << argv[0] << " [configuration file]\n";
		return (EINVAL);
	}

	signal(SIGINT, stopAllServerLoops);
	signal(SIGTERM, stopAllServerLoops);

	// Config config;
	// if (argc == 2)
	// 	config = Config(argv[2]);
>>>>>>> origin/rogier

	}
	try {
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		std::cerr << "Fatal Error: " << exception.what() << std::endl;
	}
	return (0);
}
