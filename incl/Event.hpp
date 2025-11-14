#ifndef EVENT_HPP
# define EVENT_HPP

// C
# include <stddef.h>
// C++
# include <iostream>
// webserv
# include "Socket.hpp"
// wrappers
# include "WrapEpoll.hpp"

class	Event : public epoll_event
{
	public:
		using	Events	= enum WrapEpoll::Events;

	public:
		Event() = default;
		Event(Events eventTypes, int fd);
		Event(Event const &other) = default;
		Event(Event &&other) = default;
		~Event() = default;

	public:
		operator int() const;

	public:
		void	handle();
		bool	isWeird() const;

	private:
		virtual void	_in();
		virtual void	_out();
};

#endif
