#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int socketFd, Config::Listener const &config)
	:	Event(socketFd, Epoll::Events::In)
	,	r_config(config)
{
	std::cout << "Client " << data.fd << " \e[34mConstructed\e[0m\n";
}

ClientEvent::~ClientEvent()
{
	std::cout << "Client " << data.fd << " \e[31mDestructed\e[0m\n";
}

void
ClientEvent::_in()
{
	ssize_t	recieved = Socket::recv(data.fd, _requestBuffer);

	if (recieved == 0)
		throw CloseConnection(data.fd);

	if (_requestBuffer.find("\r\n\r\n") == std::string::npos)
		return;

	// if content-length given, wait to recv whole request body (maybe start time-out clock?)

	std::cout << "Client " << data.fd << " Request:\n" << _requestBuffer << "\n";

	_request.parse(std::move(_requestBuffer)); // leaves buffer empty

	// MIDDLE request processing placeholder
	if (_request.getVersion() != "0.9")
		_response.setVersion("1.0");

	std::string indexContent = readFile("./www/index.html");
	if (indexContent.empty()) {
		_response.err(404);
	} else {
		_response.setEntityBody(indexContent);
	}

	_responseBuffer = _response.toString();

	throw ReadyToSend(data.fd);
}

void
ClientEvent::_out()
{
	ssize_t	sent = Socket::send(data.fd, _responseBuffer);

	if (sent == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // try again later
		else throw CloseConnection(data.fd);
	}

	_responseBuffer.erase(0, sent);

	if (_responseBuffer.empty())
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
