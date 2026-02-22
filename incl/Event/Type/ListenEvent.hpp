#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Config.hpp"
# include "EventHandlers.hpp"
# include "ClientEvent.hpp"
# include "Epoll.hpp"

class ListenEvent : public Event
{
	public:
		ListenEvent(ListenEvent const &) = delete;
		ListenEvent(ListenEvent &&) = delete;
		ListenEvent(int socketFd, Epoll &epoll, Config::Listener &config);
		~ListenEvent() = default;

	private:
		Epoll	&r_epoll;

		Config::Listener	_config;

	private:
		void	_in() override;
};

#endif
