#include "Server.hpp"

Server::Server()
	:	_socket(socket(AF_INET, SOCK_STREAM, 0))
	,	_epoll(_socket)
{
	if (_socket == -1)
		perror("socket()");

	sockaddr_in	serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		perror("bind()");

	if (listen(_socket, 5) == -1)
		perror("listen()");

}

Server::~Server()
{
	close(_socket);
}
