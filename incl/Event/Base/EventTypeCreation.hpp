#ifndef EVENTTYPECREATION_HPP
# define EVENTTYPECREATION_HPP

// webserv
# include "Event.hpp"
# include "EventRegistrar.hpp"
# include "EventTypes.hpp"

# define newEvent(Type)											\
\
class Type##Event : public Event								\
{																\
	public:														\
		Type##Event() = default;								\
		Type##Event(Event const &base) : Event(base) {};		\
	private:													\
		void	_in() const override;							\
		void	_out() const override;							\
};																\
\
static EventRegistrar<Type##Event>	Type##Registration;			\

#endif
