#ifndef EVENTHANDLERS_HPP
# define EVENTHANDLERS_HPP

// C++
# include <memory>
# include <map>
// webserv
# include "Event.hpp"

class	EventHandlers// make this a namespace
{
	public:
		EventHandlers() = delete;

	private:
		static std::map<int, std::unique_ptr<Event>>	_handlers;

	public:
		// template <typename Type>
		// static void		specify(Type const &event);

		template <typename Type, typename... Args>
		static Type		&create(int fd, Args&&... args);
		static Event	*get(int fd);
		// static void		destroy(int fd);
};

# include "EventHandlers.tpp"

#endif
