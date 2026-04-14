#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C
# include <sys/stat.h>
# include <sys/wait.h>
// C++
# include <iostream>
# include <functional>
# include <filesystem>
// webserv
# include "Config.hpp"
# include "EventHandlers.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "CGInboxEvent.hpp"
# include "Logger.hpp"
# include "IO.hpp"

class	CGInboxEvent;

class ClientEvent : public Event
{
	public:
		using LocationMap	= std::map<std::string, Config::Listener::Location>;
		using PageMap		= std::map<u_int16_t, std::string>;
		struct	Target	{
			std::string	location;
			std::string	root;
			std::string	file;
			std::string	extension;
		};

		const std::map<std::string, std::string>	SupportedCGIExtensions = {
			{".py", "/usr/bin/python3"},
			{".sh", "/bin/bash"},
			{".php", "/usr/bin/php-cgi"},
			// {".pl", "/usr/bin/perl"},
		};

	private:
		static const std::string	HeaderEnd;

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

		bool			_receivedHead;

		Target			_target;

		pid_t			_cgild;

	public:
		void	youHaveGotMail(std::string &CGIoutput);

	private:
		void	_in() override;
		void	_out() override;

		void	_cgi(Config::Listener::Location const &location);
		void	_get(Config::Listener::Location const &location);
		void	_post(Config::Listener::Location const &location);
		void	_delete(Config::Listener::Location const &location);

	private:
		void	_receiveHead();
		void	_receiveBody();

		void	_processRequest();
		void	_finalise();

		int			_URIdentification();
		std::string	_collapseSlashes(std::string const &rawURI) const;

		char	**setupEnvironment(std::string const &scriptPath) const;
		void	parseMailHeaders(std::string const &headerStream);

	private:
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

	private:
		class	HttpError : std::exception {
			public:
				HttpError(u_int16_t = 0);
				~HttpError() = default;

			private:
				u_int16_t	_status;
				std::string	_what;

			public:
				u_int16_t	status() const;
				const char	*what() const noexcept override;
		};
};

#endif
