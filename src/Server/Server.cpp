#include "Server.hpp"

Server::Server(Config &config)
{
	for (Config::Server &listener : config.servers)
	{
		int	socketFd = ListenSocket(listener.port);
		EasyPrint(socketFd);

		ListenEvent	&listen =
			EventTypes::create<ListenEvent>(socketFd, Epoll::Events::In, _epoll);

		EasyThrow(_epoll.ctl(Epoll::Ctl::Add, listen));

		std::cout << "listening on port " << listener.port << "\n";
	}
}

void
Server::run()
{
	std::cout << "Server running...\n";
	while (_pleaseShutDown == false)
	{
		try {
			for (epoll_event &unknown : _epoll.wait(10000))
			{
				if (unknown.events & (Epoll::Events::Err | Epoll::Events::Hup))
					_closeConnection(unknown.data.fd);
				else try {
					EventTypes::get(unknown.data.fd)->handle();
				} catch (Event::CloseConnection &badEvent) {
					_closeConnection(badEvent.fd());
				}
			}
		}
		catch	(std::runtime_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "'tis but a scratch: " << exception.what() << std::endl;
		}
		catch (EventTypes::NotRegistered &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Write better code: " << exception.what() << std::endl;
			break;
		}
		catch (std::exception &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Something unexpected excepted: " << exception.what() << std::endl;
			throw exception; // throw to main for quick debug, in production the loop should not be broken
		}
	}
	LOGGER(log("Controlled Server Shutdown\n"));
	std::cout << "Server shutting down...\n";
}

void
Server::_closeConnection(int fd)
{
	EasyThrow(_epoll.ctl(Epoll::Ctl::Del, fd));
	EasyThrow(close(fd));

	std::cout << "Client " << fd << " disconnected.\n";
}
