#include "ListenEvent.hpp"

ListenEvent::ListenEvent(int socketFd, Epoll::Events events, Epoll &epoll, Config::Server &config)
	:	Event(socketFd, events)
	,	r_epoll(epoll)
	,	_config(config)
{}

void
ListenEvent::_in()
{
	ClientEvent	&client =
		EventTypes::create<ClientEvent>(
			Socket::accept(data.fd), _config);
	EasyThrow(r_epoll.ctl(Epoll::Ctl::Add, client));

	std::cout << "Client " << client.data.fd << " \e[32mSuccessfully Connected.\e[0m\n";
}
