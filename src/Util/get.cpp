#include <sys/types.h>
#include <sys/socket.h>
#include <stack>
#include <iostream>

#include "get.hpp"
#include "Logger.hpp"
#include "TokenStream.hpp"

std::ifstream
getFile(std::string configFilePath)
{
	if (configFilePath.rfind(".conf") == std::string::npos)
		throw std::runtime_error("incorrect file extension: " + configFilePath);

	std::ifstream file(configFilePath);
	if (!file.is_open())
		throw std::runtime_error("could not open file: " + configFilePath);

	return (file);
}

//moet nog wel aangepast, maar even hier geparkeerd.
// std::ifstream getHttpRequest(int clientFd)
// {
// 	char		buffer[4096];
// 	std::string	request;
// 	std::string line;
// 	while (true)
// 	{
// 		ssize_t	n	=	recv(clientFd, buffer, 4096, 0);
// 		request.append(buffer);
// 		if (request.find("\r\n\r\n") != std::string::npos)
// 			break;
// 	}
// }