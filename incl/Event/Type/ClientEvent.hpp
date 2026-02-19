#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

// C++
# include <iostream>
// webserv
# include "Config.hpp"
# include "EventHandlers.hpp"
# include "Http_v1_0.hpp"
# include "ErrorPages.hpp"
# include "CGI.hpp"
# include "Logger.hpp"
# include "IO.hpp"

class ClientEvent : public Event
{
	public:
		ClientEvent() = default;// delete?
		ClientEvent(ClientEvent const &) = delete;
		ClientEvent(ClientEvent &&) = delete;
		ClientEvent(int socketFd, Config::Listener const &config);
		~ClientEvent();

	private:
		Http::Request	_request;
		Http::Response	_response;

		std::string	_requestBuffer;
		std::string	_responseBuffer;

		Config::Listener const	&r_config;


	private:
		void	_in() override;
		void	_out() override;

/* 	private:
		void	readRequest();
		void	parseRequest();
		void	generateResponse();
		void	sendResponse();
		void	CGIRequest();
		// void	CGIRelay(); */
};

#endif
