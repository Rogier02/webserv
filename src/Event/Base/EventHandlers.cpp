#include "EventHandlers.hpp"

std::map<int, std::unique_ptr<Event>>	EventHandlers::_handlers;

Event *
EventHandlers::get(int fd)
{
	std::map<int, std::unique_ptr<Event>>::iterator it = _handlers.find(fd);
	if (it == _handlers.end())
		throw std::logic_error("no event type is specified for file descriptor");
	return (it->second.get());
}

void	EventHandlers::erase(int fd) {
	_handlers.erase(fd);
}
