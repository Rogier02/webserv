#include "EventTypes.hpp"

std::vector<std::unique_ptr<Event>>		EventTypes::_prototypes;
std::map<int, std::unique_ptr<Event>>	EventTypes::_handlers;

void
EventTypes::add(std::unique_ptr<Event> prototype)
{
	_prototypes.push_back(std::move(prototype));
}

Event*
EventTypes::get(int fd)
{
	std::map<int, std::unique_ptr<Event>>::iterator it = _handlers.find(fd);
	if (it == _handlers.end())
		throw NotRegistered();
	return (it->second.get());
}

const char *
EventTypes::NotRegistered::what()
const throw() {
	return ("file descriptor is not registered to a specific type of event");
}
