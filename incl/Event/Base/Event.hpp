#ifndef EVENT_HPP
# define EVENT_HPP

// C
# include <stddef.h>
# include <sys/epoll.h>
// C++
# include <iostream>
// webserv
# include "Epoll.hpp"
# include "Socket.hpp"

class	Event : public epoll_event
{
	public:
		enum Signal {
			OK,
			Write,
			Close,
		};

	public:
		Event() = default;
		Event(Event const &) = default;
		Event(Event &&) = default;
		Event(int fd, u_int32_t eventTypes);
		virtual ~Event() = default;

	public:
		Signal	handle();

	private:
		virtual void	_in();
		virtual void	_out();

	protected:
		Signal	_signal;
};

#endif
