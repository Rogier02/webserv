// webserv
# include "EventTypes.hpp"

template <typename Type>
void EventTypes::specify(Event const &event)
{
	_handlers[event.data.fd] = std::make_unique<Type>(event);
}
