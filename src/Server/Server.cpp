#include "Server.hpp"

Server::Server(Config &config)
{
	for (Config::Listener &listener : config.listeners)
	{
		int	socketFd = Socket::create(listener.port);

		EasyPrint(socketFd);
		EasyPrint(listener.port);

		ListenEvent	&listen =
			EventHandlers::create<ListenEvent>(
				socketFd, _epoll, listener);
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
									| Epoll::Events::RdH))
					_closeConnection(fd);
				else
					EventHandlers::get(fd)->handle();
			}
		}
		catch	(std::runtime_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "'tis but a scratch: "
				<< exception.what() << std::endl;
		}
		catch (std::logic_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Write better code: "
				<< exception.what() << std::endl;
			break;
		}
		catch (std::exception &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Something unexpected excepted: "
				<< exception.what() << std::endl;
			throw exception;
		}
		// TODO: time out requests that haven't sent for a while
	}
	LOGGER(log("Controlled Server Shutdown\n"));
	std::cout << "Server shutting down...\n";
}

void
Server::_closeConnection(int fd)
{
	EventHandlers::erase(fd);
	std::cout << "Client " << fd << " \e[33mSuccessfully Disconnected.\e[0m\n";
}
