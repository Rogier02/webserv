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
		Event() = default;
		Event(Event const &) = default;
		Event(Event &&) = default;
		Event(int fd, Epoll::Events eventTypes);
		virtual ~Event() = default;

	public:
		void	handle();

	private:
		virtual void	_in();
		virtual void	_out();

	public:
		class	CloseConnection : public std::runtime_error {
			public:
				CloseConnection(int fd);
				int	fd();
			private:
				int	_fd;
		};
};

#endif
