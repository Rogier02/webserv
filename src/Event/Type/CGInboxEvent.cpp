#include "CGInboxEvent.hpp"

CGInboxEvent::CGInboxEvent(int fd, ClientEvent &client, Epoll &epoll, Config::Listener const &config)
	:	Event(fd, Epoll::Events::In | Epoll::Events::RdH, epoll, config)
	,	r_client(client)
{}

CGInboxEvent::~CGInboxEvent()
{
	close(data.fd);
	std::cout << "CGInbox pipe read end " << data.fd << " \e[3mClosed\e[0m\n";
}

void
CGInboxEvent::_in()
{
	// read pipe until EOF
	// send output to client directly
}
