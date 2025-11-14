#include "Event.hpp"

Event::Event(Events eventTypes, int fd)
{
	events = eventTypes;
	data.fd = fd;
}

bool	Event::isWeird() const {
	return (events & (Events::Err | Events::Hup));
}

Event::operator int() const {
	return (data.fd);
}

void	Event::handle()
{
	if (events & Events::In)
		_in();
	if (events & Events::Out)
		_out();
}

void	Event::_in()
{
	std::string	request = Socket::recv(data.fd);
	if (request.empty()) {
		throw data.fd;
	} else {
		std::cout << "Client Request:\n" << request << std::endl;
	}
}

void	Event::_out()
{}
