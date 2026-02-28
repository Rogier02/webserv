#ifndef EVENT_HPP
# define EVENT_HPP

// C
# include <stddef.h>
// C++
# include <iostream>
// webserv
# include "Epoll.hpp"
# include "Config.hpp"

class	Event : public epoll_event
{
	public:
		Event() = default;
		Event(Event const &) = default;
		Event(Event &&) = default;
		Event(int fd, u_int32_t eventTypes, Epoll &epoll, Config::Listener const &config);
		virtual ~Event();

	protected:
		Epoll					&r_epoll;
		Config::Listener const	&r_config;

	public:
		void	handle();

	private:
		virtual void	_in();
		virtual void	_out();

	protected:
		void	_mod(u_int32_t eventTypes);
};

#endif
