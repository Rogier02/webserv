#ifndef SERVERTOCGIEVENT_HPP
# define SERVERTOCGIEVENT_HPP

// C++
# include <iostream>
// webserv
# include "EventHandlers.hpp"
# include "ClientEvent.hpp"
# include "Logger.hpp"

class	ClientEvent;

class ServerToCGIEvent : public Event
{
	public:
		// ServerToCGIEvent() = default;
		ServerToCGIEvent(ServerToCGIEvent const &) = delete;
		ServerToCGIEvent(ServerToCGIEvent &&) = delete;
		ServerToCGIEvent(int fd, Epoll &epoll, Config::Listener const &config, const std::string& request_body);
		~ServerToCGIEvent();

	private:
	
		std::string	_request_body;
		size_t _bytes_written;

	private:
		void	_out() override;
};

#endif
