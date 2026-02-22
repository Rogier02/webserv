#include "Server.hpp"

Server::Server(Config &config)
{
	for (Config::Listener &listener : config.listeners)
	{
		int	socketFd = Socket::create(listener.port);
		_listenSockets.push_back(socketFd);
		EasyPrint(socketFd);
		EasyPrint(listener.port);

		ListenEvent	&listen =
			EventHandlers::create<ListenEvent>(
				socketFd, _epoll, listener);

		EasyThrow(_epoll.ctl(Epoll::Ctl::Add, listen));
	}
}

// Server::~Server()
// {
// 	for (int fd : _listenSockets)
// 		close(fd);
// }

void
Server::run()
{
	std::cout << "Server running...\n";
	while (_pleaseShutDown == false)
	{
		try {
			for (epoll_event &unknown : _epoll.wait(10000))
			{
				int	fd = unknown.data.fd;

				if (unknown.events & (Epoll::Events::Err
									| Epoll::Events::Hup
									| Epoll::Events::RdH)) {
					_closeConnection(fd);
					continue;
				}

				Event::Signal	signal = EventHandlers::get(fd)->handle();

				if (signal == Event::Signal::Write)
					_readyToSend(fd);
				if (signal == Event::Signal::Close)
					_closeConnection(fd);
			}
		}
		catch	(std::runtime_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "'tis but a scratch: " << exception.what() << std::endl;
		}
		catch (std::logic_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Write better code: " << exception.what() << std::endl;
			break;
		}
		catch (std::exception &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Something unexpected excepted: " << exception.what() << std::endl;
			throw exception;
		}
		// TODO: time out requests that haven't sent for a while
	}
	LOGGER(log("Controlled Server Shutdown\n"));
	std::cout << "Server shutting down...\n";
}

void
Server::_readyToSend(int fd)
{
	Event	*event	= EventHandlers::get(fd);
	event->events	= Epoll::Events::Out | Epoll::Events::RdH;

	EasyThrow(_epoll.ctl(Epoll::Ctl::Mod, *event));

	std::cout << "Client " << fd << " \e[33mSuccessfully Registered For Sending\e[0m\n";
}

// closing the connection leaves the pointer to the Event invalid
void
Server::_closeConnection(int fd)
{
	EasyThrow(_epoll.ctl(Epoll::Ctl::Del, fd));
	EventHandlers::erase(fd);
	EasyThrow(close(fd));

	std::cout << "Client " << fd << " \e[33mSuccessfully Disconnected.\e[0m\n";
}
