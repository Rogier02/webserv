#include "CGOutboxEvent.hpp"

CGOutboxEvent::CGOutboxEvent(int fd, Epoll &epoll, Config::Listener const &config, const std::string& request_body)
	:	Event(fd, Epoll::Events::Out, epoll, config)
	, 	_inputBuffer(request_body)
{
	LOG(Memory, "  CGOutboxEvent Constructed: " + std::to_string(data.fd));
}

CGOutboxEvent::~CGOutboxEvent()
{
	LOG(Memory, "  CGOutboxEvent Destructed: " + std::to_string(data.fd));
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
		EventHandlers::erase(data.fd);
	}
}
