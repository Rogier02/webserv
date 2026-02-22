#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Config.hpp"
# include "EventHandlers.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "CGI.hpp"
# include "CGInboxEvent.hpp"
# include "Logger.hpp"
# include "IO.hpp"

class	CGInboxEvent;

class ClientEvent : public Event
{
	public:
		using	LocationMap	= std::map<std::string, Config::Listener::Location>;
		using	PageMap		= std::map<u_int16_t, std::string>;

	public:
		// ClientEvent() = default;
		ClientEvent(ClientEvent const &) = delete;
		ClientEvent(ClientEvent &&) = delete;
		ClientEvent(int fd, Config::Listener const &config);
		~ClientEvent();

	private:
		Http::Request	_request;
		Http::Response	_response;

		std::string		_requestBuffer;
		std::string		_responseBuffer;

		CGInboxEvent	*_CGInbox;

		Config::Listener const	&r_config;

	private:
		void	_in() override;
		void	_out() override;

		void	_get();
		void	_post();
		void	_delete();

		void	_youHaveGotMail(std::string &CGIoutput);

		LocationMap::const_iterator	_findLocation(std::string URI);
};

#endif
