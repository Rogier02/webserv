#include "Server.hpp"

Server::Server()
	:	_listenSocket(_port)
{
	Event	event(EpollEvents::In, _listenSocket);
	_epoll.ctl(Epoll::Ctl::Add, event);
}

void
Server::run()
{
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_pleaseShutDown == false)
	{
		try {
			for (epoll_event &unknown : _epoll.wait())
			{
				if (unknown.data.fd == _listenSocket)
					_addEvent();
				else if (unknown.events & (EpollEvents::Err | EpollEvents::Hup))
					_delEvent(unknown.data.fd);
				else try {
					Event	*event = EventTypes::get(unknown.data.fd);
					event->handle();
				} catch (int closing) {
					_delEvent(closing);
				}
			}
		}
		catch	(std::runtime_error &exception) {
			Logger::log(exception.what());
			std::cerr << "'tis but a scratch: " << exception.what() << std::endl;
		}
		catch (EventTypes::NotRegistered &exception) {
			Logger::log(exception.what());
			std::cerr << "Write better code: " << exception.what() << std::endl;
			break;
		}
		catch (std::exception &exception) {
			Logger::log(exception.what());
			std::cerr << "Something unexpected excepted: " << exception.what() << std::endl;
			throw exception;
		}
	}
	Logger::log("Controller Server Shutdown\n");
	std::cout << "Server shutting down...\n";
}

void
Server::_addEvent()
{
	Event	event(EpollEvents::In, _listenSocket.accept());

	_epoll.ctl(Epoll::Ctl::Add, event);
	std::cout << "Client " << event.data.fd << " connected.\n";
}

void
Server::_delEvent(int fd)
{
	_epoll.ctl(Epoll::Ctl::Del, fd);
	EasyThrow(close(fd));
	std::cout << "Client " << fd << " disconnected.\n";
}
