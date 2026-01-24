#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Config.hpp"
# include "EventTypes.hpp"
# include "ClientEvent.hpp"
# include "Epoll.hpp"

class ListenEvent : public Event
{
	public:
		ListenEvent(int socketFd, Epoll::Events events, Epoll &epoll, Config::Server &config);

	private:
		Epoll			&r_epoll;

		Config::Server	_config;

	private:
		void	_in() override;
};

#endif
