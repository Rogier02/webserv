#ifndef CLIENTSTATE_HPP
# define CLIENTSTATE_HPP

#include <string>
#include <ctime>
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
		EXECUTING_CGI = 3,
		SENDING_RESPONSE = 4,
		DONE = 5
	};

	State			_currentState;
	time_t			_lastActivity;

	// Request handling
	std::string 	_requestBuffer;
	HttpRequest		_request;
	bool			_requestComplete;

	// Response handling
	std::string 	_responseBuffer;
	HttpResponse	_response;
	size_t			_responseBytesSent;

	// CGI handling 
	pid_t			_cgiPid;
	int				_cgiPipeReadFd;
	std::string		_cgiOutput;

	//Constructor
	ClientState()
		:	_currentState(READING_REQUEST)
		,	_lastActivity(std::time(nullptr))
		,	_requestComplete(false)
		,	_responseBytesSent(0)
	{}

	// Check if request is complete (has \r\n\r\n)
	bool isRequestComplete() const {
		return _requestBuffer.find("\r\n\r\n") != std::string::npos;
	}
	
	// check if client has timedout (30s)
	bool isTimedout(time_t now, int timeoutSeconds = 30) const {
		return (now - _lastActivity) > timeoutSeconds;
	}

	// Update last activity timestamp
	void updateActivity() {
		_lastActivity = std::time(nullptr);
	}

	void appendToRequestBuffer(const std::string& data) {
		_requestBuffer.append(data);
		updateActivity();
	}

	void reset() {
		_requestBuffer.clear();
		_responseBuffer.clear();
		_responseBytesSent = 0;
		_requestComplete = false;
		_currentState = READING_REQUEST;
		_cgiPid = -1;
		_cgiPipeReadFd = -1;
		_cgiOutput.clear();
		updateActivity();
	}

	size_t getRemainigResponseBytes() const {
		return _responseBuffer.length() - _responseBytesSent;
	}
};

#endif