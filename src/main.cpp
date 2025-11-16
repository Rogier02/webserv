#include <errno.h>
#include <stdio.h>
<<<<<<< HEAD
#include <exception>
#include <iostream>
#include <ifstream>

=======
#include <stdexcept>

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

	// Config config;
	// if (argc == 2)
	// 	config = Config(argv[2]);
>>>>>>> origin/rogier

	}
	try {
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		std::cerr << exception.what() << std::endl;
	}
	return (0);
}
