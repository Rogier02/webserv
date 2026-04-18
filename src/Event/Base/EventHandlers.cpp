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

void	EventHandlers::iter(std::function<void(Event&)> func)
{
	for (std::map<int, std::unique_ptr<Event>>::iterator it = _handlers.begin(); it != _handlers.end(); ++it)
		func(*it->second);
}