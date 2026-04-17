#ifndef CGOUTBOXEVENT_HPP
# define CGOUTBOXEVENT_HPP

// C++
# include <iostream>
// webserv
# include "EventHandlers.hpp"
# include "ClientEvent.hpp"
# include "Logger.hpp"

class	ClientEvent;

class CGOutboxEvent : public Event
{
	public:
		// CGOutboxEvent() = default;
		CGOutboxEvent(CGOutboxEvent const &) = delete;
		CGOutboxEvent(CGOutboxEvent &&) = delete;
		CGOutboxEvent(int fd, Epoll &epoll, Config::Listener const &config, const std::string& request_body);
		~CGOutboxEvent();

	private:
		std::string	_inputBuffer;

	private:
		void	_out() override;
};

#endif
