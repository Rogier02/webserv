#ifndef EVENTTYPECREATION_HPP
# define EVENTTYPECREATION_HPP

// webserv
# include "Event.hpp"
# include "EventRegistrar.hpp"
# include "EventTypes.hpp"

# define newEvent(Type)											\
class Type##Event : public Event								\
{																\
	public:														\
		explicit Type##Event(Event const &base);				\
	public:														\
		void	_in() const override;							\
		void	_out() const override;							\
};																\
\
static EventRegistrar<Type##Event>	Type##Registration();		\
\
inline	Type##Event::Type##Event(Event const &base)				\
	:	Event(base)												\
{																\
	EventTypes::specify(std::make_unique<Type##Event>(base));	\
}

#endif
