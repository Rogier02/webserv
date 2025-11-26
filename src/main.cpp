#include <errno.h>
#include <stdio.h>
#include <stdexcept>
#include <signal.h>

#include "Server.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"
#include "parsing.hpp"

std::atomic<bool>	_pleaseShutDown(false);

void	stopAllServerLoops(int)
{
	_pleaseShutDown = true;
	std::cerr << std::endl;
}

int	main(int argc, char **argv)
{
	std::cout << "First thing to do is start\n" << std::endl;
	if (argc > 2) {
		std::cout << "Usage: " << argv[0] << " [configuration file]\n";
		return (EINVAL);
	}
	std::cout << "check 1 complete\n";
	signal(SIGINT, stopAllServerLoops);
	signal(SIGTERM, stopAllServerLoops);

	Config config;
	if (argc == 2){
		std::cout << "check 2 complete. argc = 2\n";
		config.getFileContent(argv[2]);
	}
	else{
		std::cout << "check 2 complete. argc != 2\n";
		config.getFileContent("default.conf");
	}


	// try {
	// 	Server	server;
	// 	server.run();
	// } catch (std::exception &exception) {
	// 	std::cerr << "Fatal Error: " << exception.what() << std::endl;
	// }
	return (0);
}