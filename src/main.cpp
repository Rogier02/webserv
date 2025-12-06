#include <errno.h>
#include <stdio.h>
#include <stdexcept>
#include <signal.h>

#include "Server.hpp"
#include "Config.hpp"
#include "Parse.hpp"
#include "HttpRequest.hpp"

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
	if (configFilePath.rfind(".conf") == std::string::npos)
		throw std::runtime_error("incorrect file extension: " + configFilePath);

	std::ifstream file(configFilePath);
	if (!file.is_open())
		throw std::runtime_error("could not open file: " + configFilePath);

signal(SIGINT, stopAllServerLoops);
signal(SIGTERM, stopAllServerLoops);

	try {
		// Parse	parser(configFilePath);
		Parse	parser(file, CONFIGFILE);
		Config	config = parser.config();
			file.close();
		std::cout << config;

		// Server	server(configFilePath);
		Server	server;
		server.run();
	} catch (std::exception &exception) {
		std::cerr << "Fatal Error: " << exception.what() << std::endl;
	}
	return (0);
}
