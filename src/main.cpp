#include <errno.h>
#include <stdio.h>
#include <stdexcept>

#include "Server.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"

std::atomic<bool>	_running(false);

void	shutdown(int)
{
	_running = false;
	std::cerr << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc > 2) {
		std::cout << "Usage: " << argv[0] << " [configuration file]\n";
		return (EINVAL);
	}

	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown);

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
