#include "CGInboxEvent.hpp"

CGInboxEvent::CGInboxEvent(int fd, ClientEvent &client, Epoll &epoll, Config::Listener const &config)
	:	Event(fd, Epoll::Events::In | Epoll::Events::RdH, epoll, config)
	,	r_client(client)
{}

CGInboxEvent::~CGInboxEvent()
{
	std::cout << "CGInbox pipe read end " << data.fd << " \e[31mClosed\e[0m\n";
}

void
CGInboxEvent::_in()
{
	const ssize_t	received = Socket::recv(data.fd, _outputBuffer);

	if (received == -1 || _outputBuffer.empty()) {
		_outputBuffer = strerror(errno);// with status: 500 probably?
	}

	// read pipe until EOF
	// send output to client directly
	// how to format this output? does it need headers? does the script provide them?
	r_client.youHaveGotMail(_outputBuffer);

	EventHandlers::erase(data.fd);
}
