#include "ListenEvent.hpp"

ListenEvent::ListenEvent(int socketFd, Epoll::Events events, Epoll &epoll)
	:	Event(socketFd, events)
	,	r_epoll(epoll)
{}

void
ListenEvent::_in()
{
	ClientEvent	client =
		EventTypes::create<ClientEvent>(Socket::accept(data.fd), Epoll::Events::In);
	EasyThrow(r_epoll.ctl(Epoll::Ctl::Add, client));

	std::cout << "Client " << client.data.fd << " connected.\n";
}
