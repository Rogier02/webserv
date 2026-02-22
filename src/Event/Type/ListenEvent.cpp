#include "ListenEvent.hpp"

ListenEvent::ListenEvent(int socketFd, Epoll &epoll, Config::Listener &config)
	:	Event(socketFd, Epoll::Events::In | Epoll::Events::RdH)
	,	r_epoll(epoll)
	,	_config(config)
{}

void
ListenEvent::_in()
{
	ClientEvent	&client =
		EventHandlers::create<ClientEvent>(
			Socket::accept(data.fd), _config);
	EasyThrow(r_epoll.ctl(Epoll::Ctl::Add, client));

	std::cout << "Client " << client.data.fd << " \e[33mSuccessfully Connected.\e[0m\n";
}
