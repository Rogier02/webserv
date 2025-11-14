#include "Server.hpp"

std::atomic<bool> Server::_running(false);

Server::Server()
	:	_socket(_port)
	,	_epoll(_socket)
{
	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown);
}

void
Server::run()
const {
	_running = true;
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_running)
	{
		try {
			for (Event &event : _epoll.wait())
			{
				if (event.isClosed())
					_epoll.ctl(Epoll::Ctl::Del, event.data.fd);
				else if (event.data.fd == _socket)
					newClient();
				else
					event.handle();
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
Server::shutdown(int)
{
	_running = false;
	std::cerr << std::endl;
}

void
Server::newClient()
const {
	Event	event(Event::Events::In, _socket.accept());

	_epoll.ctl(Epoll::Ctl::Add, event);
	std::cout << "New client connected.\n";
}
