#include "CGInboxEvent.hpp"

CGInboxEvent::CGInboxEvent(int fd, Epoll &epoll, Config::Listener const &config, ClientEvent &client)
	:	Event(fd, Epoll::Events::In, epoll, config)
	,	r_client(client)

{}

CGInboxEvent::~CGInboxEvent()
{
	std::cout << "CGInbox " << data.fd << "\e[34m Destructed (pipe read end)\e[0m\n";
}

void
CGInboxEvent::_in()
{
	const ::ssize_t	received = IO::read(data.fd, _outputBuffer);
	if (received != 0)
		return;

	r_client.youHaveGotMail(_outputBuffer);
	EasyPrint("");
	EventHandlers::erase(data.fd);
}
