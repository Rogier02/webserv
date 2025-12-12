#include "EventTypes.hpp"

std::map<int, std::unique_ptr<Event>>	EventTypes::_handlers;

Event*
EventTypes::get(int fd)
{
	std::map<int, std::unique_ptr<Event>>::iterator it = _handlers.find(fd);
	if (it == _handlers.end())
		throw NotRegistered();
	return (it->second.get());
}

const char *EventTypes::NotRegistered::what() const throw() {
	return ("no event type is specified for file descriptor");
}
