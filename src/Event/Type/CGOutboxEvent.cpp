#include "CGOutboxEvent.hpp"

CGOutboxEvent::CGOutboxEvent(int fd, Epoll &epoll, Config::Listener const &config, const std::string& request_body)
	:	Event(fd, Epoll::Events::Out, epoll, config)
	, 	_inputBuffer(request_body)
{}

CGOutboxEvent::~CGOutboxEvent()
{
	std::cout << "ServerToCGI " << data.fd << "\e[34m Destructed (pipe read end)\e[0m\n";
}

void
CGOutboxEvent::_out()
{
	::size_t	sent = IO::write(data.fd, _inputBuffer);

	LOG(Debug, std::to_string(sent) + " Characters Sent");

	_inputBuffer.erase(0, sent);

	if (_inputBuffer.empty())
	{
		LOG(Info, "CGOutbox " + std::to_string(data.fd) + " Completed Sending");
		std::cout << "CGOutbox " << data.fd << " \e[32mCompleted Sending\e[0m\n";
		EventHandlers::erase(data.fd);
	}
}
