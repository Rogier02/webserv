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
		using	EpollEvents	= enum WrapEpoll::Events;

	public:
		Event() = default;
		Event(EpollEvents eventTypes, int fd);
		Event(Event const &other) = default;
		Event(Event &&other) = default;
		virtual ~Event() = default;

	public:
		operator int() const;

	public:
		void	handle();

	private:
		virtual void	_in() const;
		virtual void	_out() const;
};

#endif
