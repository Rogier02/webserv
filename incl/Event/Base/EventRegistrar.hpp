#ifndef EVENTREGISTRAR_HPP
# define EVENTREGISTRAR_HPP

// C++
# include <memory>
// webserv
# include "EventTypes.hpp"

template<typename T>
class EventRegistrar
{
	public:
		EventRegistrar() {
			EventTypes::add(std::make_unique<T>());
		}
};

#endif
