#include "Server.hpp"

Server::Server(Config &config)
{
	LOG(Memory, "Server Constructed");

	for (Config::Listener &listener : config.listeners)
	{
		int	socketFd = Socket::create(listener.port);

		LOG(Info, "Socket Fd: " + std::to_string(socketFd));
		LOG(Info, "Listener Port: " + std::to_string(listener.port));

		EventHandlers::create<ListenEvent>(
			socketFd, _epoll, listener);
	}
}

Server::~Server()
{
	LOG(Memory, "Server Destructed");
}

void
Server::run()
{
	LOG(Info, "Server Running");
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
			LOG(Error, std::string("Runtime Error (continuing server loop): ") + exception.what());
		}
		catch (std::logic_error &exception) {
			LOG(Error, std::string("Logic Error (breaking server loop): ") + exception.what());
			break;
		}
		catch (std::exception &exception) {
			LOG(Error, std::string("!? Unexpected exception (breaking loose all hell): ") + exception.what());
			throw exception;
		}
		_timeOutClients();
#ifdef DEBUG
		sleep(1);
#endif
	}
	LOG(Info, "Controlled Server Shutdown\n");
}

void
Server::_closeConnection(int fd)
{
	EventHandlers::erase(fd);
	LOG(Info, "Client " + std::to_string(fd) + " Disconnected by Server");
}

void
Server::_timeOutClients()
{
	EventHandlers::iter([](Event &event) {
		ClientEvent *clientEvent = dynamic_cast<ClientEvent*>(&event);
		if (clientEvent)
			clientEvent->timeOut();
	});
}
