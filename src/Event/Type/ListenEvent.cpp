#include "ListenEvent.hpp"

ListenEvent::ListenEvent(int socketFd, Epoll &epoll, Config::Listener const &config)
	:	Event(socketFd, Epoll::Events::In, epoll, config)
{
	LOG(Memory, " ListenEvent Constructed: " + std::to_string(data.fd));
}

ListenEvent::~ListenEvent()
{
	LOG(Memory, " ListenEvent Destructed: " + std::to_string(data.fd));
}

void
ListenEvent::_in()
{
	int	clientFd = Socket::accept(data.fd);

	EventHandlers::create<ClientEvent>(
		clientFd, r_epoll, r_config);

	LOG(Info, "Client " + std::to_string(clientFd) + " Connected to Server");
}
