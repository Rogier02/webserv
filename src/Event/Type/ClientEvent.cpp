#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int socketFd, Config::Server const &config)
	:	Event(socketFd, Epoll::Events::In)
	,	r_config(config)
	,	_state(State::READING_REQUEST)
{
	std::cout << r_config.name << std::endl; // Dit kan gedelete worden!!
	std::cout << std::to_string(_state); // kan ook weg.
	std::cout << "Client " << data.fd << " \e[34mConstructed\e[0m\n";
}

ClientEvent::~ClientEvent()
{
	std::cout << "Client " << data.fd << " \e[31mDestructed\e[0m\n";
}

void
ClientEvent::_in()
{
	// IN
	_requestBuffer += Socket::recv(data.fd);
	std::cout << "Client " << data.fd << " Request:\n" << _requestBuffer << "\n";

	if (_requestBuffer.find("\r\n\r\n") == std::string::npos) {
		if (_requestBuffer.empty())
			throw CloseConnection(data.fd);
		return ; // incomplete request: we'll get em on the next pass
	}

	// _request = std::move(_requestBuffer); // leaves buffer empty
	Http::Request HttpRequest(std::move(_requestBuffer));
	std::cout << "-------------PARSED HTTP REQUEST-------------\n" << HttpRequest.toString() << "-------------PARSED HTTP REQUEST-------------\n" << std::endl;

	// MIDDLE (this should be elsewhere)
	// use HTTP request construction from buffer instead of simple _request string
/* 	try {
		HttpRequest(_requestBuffer);
		path? location? redirect? other stuff?
		Method???
	} catch (HttpRequest::ParseErrorException const &exception) {
		HttpResponse = std::move(HttpResponse::createErrorPage(400));
	} */

	Http::Response	HttpResponse("1.0");

	std::string indexContent = readFile("./defaultPages/index.html");
	/* if (indexContent.empty()) {
		HttpResponse = std::move(HttpResponse::createErrorPage(404));
	} else  */{
		HttpResponse.setEntityBody(indexContent);
	}

	// OUT
	Socket::send(data.fd, HttpResponse.toString());
	throw CloseConnection(data.fd);
}

/* void
ClientEvent::parseRequest()
{
	try {
		// call HttpRequestParser
		// _state = State::GENERATING_RESPONSE;
		generateResponse();
	} catch (const std::exception& e) {
		LOGGER(log("Parse error: " + std::string(e.what())));
		_response.setStatus(400);
		_response.setContentType("text/html");
		_response.setBody("<html><body><h1>400 Bad Request</h1></body></html");
		_state = State::SENDING_RESPONSE;
	}
} */

/* void
ClientEvent::generateResponse()
{
	std::istringstream	stream(std::move(_request));
	std::string			method, path, version;
	stream >> method >> path >> version;

	if (CGI::isCgiRequest(path)) {
		// _state = State::EXECUTING_CGI;
		// _state = State::SENDING_RESPONSE;
		sendResponse();
		std::string cgiOutput = CGI::execute(path, method, "", "");
		_response.setStatus(200);
		_response.setContentType("text/html");
		_response.setBody(cgiOutput);
	} else {
		// Non-CGI request (static files)
		// Temporary: Return 404 for any path that's not / or /cgi.py
		if (path == "/") {
			std::string indexContent = readFile("./defaultPages/index.html");
			if (indexContent.empty()) {
				_response.setStatus(404);
				_response.setContentType("text/html");
				_response.setBody(ErrorPages::getBody(404));
			} else {
				_response.setStatus(200);
				_response.setContentType("text/html");
				_response.setBody(indexContent);
			}
		} else {
			// Return 404 error
			_response.setStatus(404);
			_response.setContentType("text/html");
			_response.setBody(ErrorPages::getBody(404));
		}
		// _state = State::SENDING_RESPONSE;
		sendResponse();
	}
} */

/* void
ClientEvent::sendResponse()
{
	Socket::send(data.fd, _response.toString());

	throw CloseConnection(data.fd);
} */
