#ifndef CGINBOXEVENT_HPP
# define CGINBOXEVENT_HPP

// C++
# include <iostream>
// webserv
# include "EventHandlers.hpp"
# include "ClientEvent.hpp"
# include "Logger.hpp"

class	ClientEvent;

class CGInboxEvent : public Event
{
	public:
		// CGInboxEvent() = default;
		CGInboxEvent(CGInboxEvent const &) = delete;
		CGInboxEvent(CGInboxEvent &&) = delete;
		CGInboxEvent(int fd, ClientEvent &client, Epoll &epoll, Config::Listener const &config);
		~CGInboxEvent();

	private:
		std::string	_outputBuffer;

		ClientEvent const	&r_client;

	private:
		void	_in() override;
};

#endif
