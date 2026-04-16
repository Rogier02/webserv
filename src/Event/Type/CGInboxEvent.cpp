#include "CGInboxEvent.hpp"

CGInboxEvent::CGInboxEvent(int fd, ClientEvent &client, Epoll &epoll, Config::Listener const &config)
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
		LOG(Warning, "After you got mail");
			LOG(Warning, "After you got mail");
				LOG(Warning, "After you got mail");
	const ::ssize_t	received = IO::read(data.fd, _outputBuffer);
	if (received != 0)
		return;
	r_client.youHaveGotMail(_outputBuffer);
	LOG(Warning, "After you got mail");
	EventHandlers::erase(data.fd);
}
