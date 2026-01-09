#ifndef CLIENTSTATE_HPP
# define CLIENTSTAT_HPP

#include <string>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

/**
 * @enum ClientState::State
 * @brief State machine for client connection lifecycle
 */
struct ClientState {
	enum State : u_int8_t {
		READING_REQUEST = 0,
		PARSING_REQUEST = 1,
		GENERATING_RESPONSE = 2,
		EXECUTIING_CGI = 3,
		SENDING_RESPONSE = 4,
		DONE = 5
	};


	std::string _requestBuffer;
	HttpRequest	request;

	std::string responseBuffer;
	HttpResponse response;
	size_t	_responseBytesSent;
};

#endif