#include "Server.hpp"

Server::Server(Config &config)
{
	for (Config::Listener &listener : config.listeners)
	{
		int	socketFd = Socket::create(listener.port);

		LOG(Info, "Socket Fd: " + std::to_string(socketFd));
		LOG(Info, "Listener Port: " + std::to_string(listener.port));

		EventHandlers::create<ListenEvent>(
			socketFd, _epoll, listener);
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
				int	fd = unknown.data.fd;

				if (unknown.events & (Epoll::Events::Err
									| Epoll::Events::RdH))
					_closeConnection(fd);
				else
					EventHandlers::get(fd)->handle();
			}
		}
		catch	(std::runtime_error &exception) {
			LOG(Error, exception.what());
			std::cerr << "Runtime Error (continuing server loop): "
				<< exception.what() << std::endl;
		}
		catch (std::logic_error &exception) {
			LOG(Error, exception.what());
			std::cerr << "Logic Error: (breaking server loop)"
				<< exception.what() << std::endl;
			break;
		}
		catch (std::exception &exception) {
			LOG(Error, exception.what());
			std::cerr << "!? Unexpected exception: (breaking loose all hell)"
				<< exception.what() << std::endl;
			throw exception;
		}
		// TODO: time out requests that haven't done anything for a while
	}
	LOG(Info, "Controlled Server Shutdown\n");
	std::cout << "Server shutting down...\n";
}

void
Server::_closeConnection(int fd)
{
	EventHandlers::erase(fd);
	std::cout << "Client " << fd << " \e[33mSuccessfully Disconnected\e[0m\n";
}
