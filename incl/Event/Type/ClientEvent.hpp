#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C++
# include <iostream>
// webserv
# include "EventTypes.hpp"
# include "HttpResponse.hpp"
# include "ErrorPages.hpp"
# include "CGI.hpp"

class ClientEvent : public Event
{
	public:
		ClientEvent(int socketFd, Epoll::Events events);

	private:
		void	_in() const override;
};

#endif
