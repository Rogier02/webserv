#include "ListenEvent.hpp"

ListenEvent::ListenEvent(int socketFd, Epoll::Events events, Epoll &epoll)
	:	Event(events, socketFd)
	,	r_epoll(epoll)
{}

void
ListenEvent::_in()
const {
	// Event	newClient(Epoll::Events::In, Socket::accept(data.fd));

	// EventTypes::specify<ClientEvent>(newClient);
	// EasyThrow(r_epoll.ctl(Epoll::Ctl::Add, newClient));

	// std::cout << "Client " << newClient.data.fd << " connected.\n";
}
