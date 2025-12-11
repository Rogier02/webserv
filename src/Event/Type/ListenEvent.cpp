#include "ListenEvent.hpp"

void
ListenEvent::_in()
const {
	Event	newClient(Epoll::Events::In, Socket::accept(data.fd));

	EventTypes::specify<ClientEvent>(newClient);
	// EasyThrow(???epoll->ctl(Epoll::Ctl::Add, newClient)); // just write the event type headers...

	std::cout << "Client " << newClient.data.fd << " connected.\n";
}

void
ListenEvent::_out()
const {}
