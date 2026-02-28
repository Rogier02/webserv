#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Socket.hpp"
# include "Epoll.hpp"
# include "EventHandlers.hpp"
# include "ClientEvent.hpp"

class ListenEvent : public Event
{
	public:
		// ListenEvent() = default;
		ListenEvent(ListenEvent const &) = delete;
		ListenEvent(ListenEvent &&) = delete;
		ListenEvent(int socketFd, Epoll &epoll, Config::Listener const &config);
		~ListenEvent() = default;

	private:
		void	_in() override;
};

#endif
