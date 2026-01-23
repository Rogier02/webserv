#include <errno.h>
#include <stdio.h>
#include <stdexcept>
#include <signal.h>

#include "Server.hpp"
#include "Config.hpp"
#include "ParseConfig.hpp"
#include "HttpRequest.hpp"
#include "Validate.hpp"
#include "get.hpp"

#define CONFIGFILE 1

std::atomic<bool>	_pleaseShutDown(false);

static void
stopAllServerLoops(int)
{
	_pleaseShutDown = true;
	std::cerr << std::endl;
}

static int
usage(const char *executable)
{
	std::cout << "Usage: " << executable << " [configuration file]\n";
	return (EINVAL);
}

int	main(int argc, char **argv)
{
	if (argc > 2)
		return (usage(argv[0]));

	std::string configFilePath = "configs/default.conf";
	if (argc == 2)
		configFilePath = argv[1];
	std::ifstream file = getFile(configFilePath);

signal(SIGINT, stopAllServerLoops);
signal(SIGTERM, stopAllServerLoops);

	try {
		ParseConfig	parser(file, CONFIGFILE);
		Config		config = parser.config();
		file.close();
		// std::cout << config;
		Validate	validation(config);
		validation.validateConfig();

		Server	server;
		server.run();
	} catch (std::exception &exception) {
		std::cerr << "Fatal Error: " << exception.what() << std::endl;
	}
	return (0);
}
