#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Config.hpp"
# include "EventTypes.hpp"
# include "HttpResponse.hpp"
# include "ErrorPages.hpp"
# include "CGI.hpp"
# include "Logger.hpp"
# include "IO.hpp"

class ClientEvent : public Event
{
	public:
		enum	State : u_int8_t {
			READING_REQUEST = 0,
			PARSING_REQUEST = 1,
			GENERATING_RESPONSE = 2,
			EXECUTING_CGI = 3,
			SENDING_RESPONSE = 4,
			DONE = 5
		};

	public:
		ClientEvent(int socketFd, Config::Server const &config);
		ClientEvent(ClientEvent const &other) = delete;
		ClientEvent(ClientEvent &&other) = delete;
		~ClientEvent();

	private:
		Config::Server	r_config;

		State			_state;
		std::string		_requestBuffer;
		std::string		_request;

		// HttpRequest		_request;
		// bool			_requestComplete;

		HttpResponse	_response;

	private:
		void	_in() override;

/* 	private:
		void	readRequest();
		void	parseRequest();
		void	generateResponse();
		void	sendResponse();
		void	CGIRequest();
		// void	CGIRelay(); */
};

#endif
