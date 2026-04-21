#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdexcept>

#include "Server.hpp"
#include "Config.hpp"
#include "Parse.hpp"
#include "Validate.hpp"

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

int	main(int argc, char *argv[])
{
	if (argc > 2)
		return (usage(argv[0]));

	std::string configFilePath = "configs/default.conf";
	if (argc == 2)
		configFilePath = argv[1];

	signal(SIGINT, stopAllServerLoops);
	signal(SIGTERM, stopAllServerLoops);

	try {
		Config		config = Parse(configFilePath).config();
		Validate	Validate(config);
		LOG(Debug, "Config File:\n" + config.toString());

		Server	server(config);
		server.run();
	} catch (std::exception &exception) {
		std::cerr << "Fatal Error: " << exception.what() << std::endl;
	}
	return (0);
}
