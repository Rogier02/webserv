#include "ServerToCGIEvent.hpp"

ServerToCGIEvent::ServerToCGIEvent(int fd, Epoll &epoll, Config::Listener const &config, const std::string& request_body)
	:	Event(fd, Epoll::Events::Out, epoll, config)
	, 	_request_body(request_body)
	, 	_bytes_written(0)
{}

ServerToCGIEvent::~ServerToCGIEvent()
{
	std::cout << "ServerToCGI " << data.fd << "\e[34m Destructed (pipe read end)\e[0m\n";
}


void
ServerToCGIEvent::_out()
{
	
	// TODO: do we want to catch an error??? when write return
	_bytes_written += IO::write(data.fd, _request_body);

	EasyPrint(_bytes_written);

	if (_bytes_written >= _request_body.size())
	{
		std::string miep("\0");
		IO::write(data.fd, miep);
		EventHandlers::erase(data.fd);
	}
}
