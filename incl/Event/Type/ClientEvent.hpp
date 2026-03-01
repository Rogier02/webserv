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
		ClientEvent(int socketFd, Epoll &epoll, Config::Listener const &config);
		~ClientEvent();

	private:
		Http::Request	_request;
		Http::Response	_response;

		std::string		_requestBuffer;
		std::string		_responseBuffer;

		bool			_headersParsed;

		struct	Target	{
			std::string	location;
			std::string	root;
			std::string	file;
		}				_target;

		// CGInboxEvent	*_CGInbox;

	private:
		void	_in() override;
		void	_out() override;

		void	_processRequest();

		void	_get(Config::Listener::Location const &location);
		void	_post(Config::Listener::Location const &location);
		void	_delete(Config::Listener::Location const &location);
		void	_cgi(Config::Listener::Location const &location);

		void	_youHaveGotMail(std::string &CGIoutput);

		int			_URIdentification();
		std::string	_collapseSlashes(std::string const &rawURI) const;

		using	Method = std::function<void (Config::Listener::Location const &)>;
		std::map<std::string, Method>
		Methods = {
			{"GET",
				[this](Config::Listener::Location const &location)
				{ _get(location); }},
			// {"HEAD",
			// 	[this](Config::Listener::Location const &location)
			// 	{ _head(location); }},
			{"POST",
				[this](Config::Listener::Location const &location)
				{ _post(location); }},
			{"DELETE",
				[this](Config::Listener::Location const &location)
				{ _delete(location); }},
		};
};

#endif
