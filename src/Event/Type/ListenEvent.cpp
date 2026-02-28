#include "ListenEvent.hpp"

ListenEvent::ListenEvent(int socketFd, Epoll &epoll, Config::Listener const &config)
	:	Event(socketFd, Epoll::Events::In | Epoll::Events::RdH, epoll, config)
{}

void
ListenEvent::_in()
{
	int	socketFd = Socket::accept(data.fd);

	ClientEvent	&client =
		EventHandlers::create<ClientEvent>(
			socketFd, r_epoll, r_config);

	std::cout << "Client " << client.data.fd << " \e[33mSuccessfully Connected.\e[0m\n";
}
