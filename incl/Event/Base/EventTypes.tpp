// webserv
# include "EventTypes.hpp"

template <typename T>
void EventTypes::specify(Event const &event)
{
	_handlers[event.data.fd] = std::make_unique<T>(event);
}
