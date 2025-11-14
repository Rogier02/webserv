#include "Server.hpp"

Server::Server()
	:	_socket(_port)
	,	_epoll(_socket)
{
}

void
Server::run()
{
	_running = true;
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_running)
	{
		try {
			for (Event &event : _epoll.wait())
			{
				if (event.isWeird())
					delClient(event);
				else if (event == _socket)
					addClient();
				else try {
					event.handle();
				} catch (int closing) {
					delClient(closing);
				}
			}
		} catch	(std::runtime_error &restartRequired) {
			Logger::log(restartRequired.what());
			std::cerr << restartRequired.what() << std::endl;
		} catch (std::exception &unknownException) {
			// log unknown and throw to main? or just don't catch and let Logger class catch somehow?
			throw unknownException;
		}
	}
	std::cout << "Server shutting down...\n";
}

void
Server::addClient()
{
	Event	event(Event::Events::In, _socket.accept());

	_epoll.ctl(Epoll::Ctl::Add, event);
	std::cout << "New client connected.\n";
}

void
Server::delClient(int fd)
{
	_epoll.ctl(Epoll::Ctl::Del, fd);
	close(fd);
	std::cout << "Client " << fd << " disconnected.\n";
}
