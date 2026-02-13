#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Config.hpp"
# include "EventTypes.hpp"
# include "Http_v1_0.hpp"
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
		ClientEvent() = delete;
		ClientEvent(ClientEvent const &) = delete;
		ClientEvent(ClientEvent &&) = delete;
		ClientEvent(int socketFd, Config::Server const &config);
		~ClientEvent();

	private:
		Config::Server const	&r_config;

		State			_state;
		std::string		_requestBuffer;
		std::string		_request;

		// bool			_requestComplete;// maybe use response status 0 to mean Incomplete?

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
