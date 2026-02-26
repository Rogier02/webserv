#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C
# include <sys/stat.h>
// C++
# include <iostream>
# include <functional>
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

		void	_processRequest();

		void	_get(std::string const &resourcePath, Config::Listener::Location const &location);
		void	_post(std::string const &resourcePath, Config::Listener::Location const &location);
		void	_delete(std::string const &resourcePath, Config::Listener::Location const &location);

		void	_youHaveGotMail(std::string &CGIoutput);

		LocationMap::const_iterator	_URIdentification(std::string &resource);

		using	Method = std::function<void (std::string const &, Config::Listener::Location const &)>;
		std::map<std::string, Method>
		Methods = {
			{"GET",
				[this](std::string const &resourcePath, Config::Listener::Location const &location)
				{ _get(resourcePath, location); }},
			// {"HEAD",
			// 	[this](std::string const &resourcePath, Config::Listener::Location const &location)
			// 	{ _head(resourcePath, location); }},
			{"POST",
				[this](std::string const &resourcePath, Config::Listener::Location const &location)
				{ _post(resourcePath, location); }},
			{"DELETE",
				[this](std::string const &resourcePath, Config::Listener::Location const &location)
				{ _delete(resourcePath, location); }},
		};
};

#endif
