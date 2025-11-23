#include <errno.h>
#include <stdio.h>
#include <stdexcept>

#include "Server.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2) {
		std::cout << "Usage: " << argv[0] << " [configuration file]\n";
		return (EINVAL);
	}

	Config config;
	if (argc == 2)
		config.loadFromFile(argv[2]);

	try {
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		std::cerr << exception.what() << std::endl;
	}
	return (0);
}
