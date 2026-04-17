#include "CGInboxEvent.hpp"

CGInboxEvent::CGInboxEvent(int fd, Epoll &epoll, Config::Listener const &config, ClientEvent &client)
	:	Event(fd, Epoll::Events::In, epoll, config)
	,	r_client(client)

{
	LOG(Memory, " CGInboxEvent Constructed: " + std::to_string(data.fd));
}

CGInboxEvent::~CGInboxEvent()
{
	LOG(Memory, " CGInboxEvent Destructed: " + std::to_string(data.fd));
}

void
CGInboxEvent::_in()
{
	const ::ssize_t	received = IO::read(data.fd, _outputBuffer);
	if (received != 0)
		return;

	r_client.youHaveGotMail(_outputBuffer);
	EventHandlers::erase(data.fd);
}
