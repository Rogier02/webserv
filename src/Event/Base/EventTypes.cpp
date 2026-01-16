#include "EventTypes.hpp"

std::map<int, std::unique_ptr<Event>>	EventTypes::_handlers;

Event*
EventTypes::get(int fd)
{
	std::map<int, std::unique_ptr<Event>>::iterator it = _handlers.find(fd);
	if (it == _handlers.end())
		throw std::logic_error("no event type is specified for file descriptor");
	return (it->second.get());
}
