#include <errno.h>
#include <stdio.h>
#include <stdexcept>
#include <signal.h>

#include "Server.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"

std::atomic<bool>	_pleaseShutDown(false);

void	stopAllServerLoops(int)
{
	_pleaseShutDown = true;
	std::cerr << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc > 2) {
		std::cout << "Usage: " << argv[0] << " [configuration file]\n";
		return (EINVAL);
	}

	signal(SIGINT, stopAllServerLoops);
	signal(SIGTERM, stopAllServerLoops);

	// Config config;
	// if (argc == 2)
	// 	config = Config(argv[2]);

	try {
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		std::cerr << "Fatal Error: " << exception.what() << std::endl;
	}
	return (0);
}
