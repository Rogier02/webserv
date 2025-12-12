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
		Event(Epoll::Events eventTypes, int fd);
		virtual ~Event() = default;

	public:
		void	handle();

	private:
		virtual void	_in() const;
		virtual void	_out() const;

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
