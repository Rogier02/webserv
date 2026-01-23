#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

// C++
# include <iostream>
// webserv
# include "EventTypes.hpp"
# include "ClientEvent.hpp"
# include "Epoll.hpp"

class ListenEvent : public Event
{
	public:
		ListenEvent(int socketFd, Epoll::Events events, Epoll &epoll);

	private:
		Epoll	&r_epoll;

	private:
		void	_in() override;
};

#endif
