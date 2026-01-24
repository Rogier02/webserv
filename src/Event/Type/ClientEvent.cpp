#include "ClientEvent.hpp"

#include <fstream>
#include <sstream>

static std::string
readFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		return ("");
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return (buffer.str());
}

ClientEvent::ClientEvent(int socketFd, Config::Server const &config)
	:	Event(socketFd, Epoll::Events::In)
	,	r_config(config)
	,	_state(State::READING_REQUEST)
{
	std::cout << "Client " << data.fd << " Constructed\n";
}

ClientEvent::~ClientEvent()
{
/* 	// // Close CGI pipe if open
	// if (client._cgiPipeReadFd != -1) {
	// 	close(client._cgiPipeReadFd);
	// 	_epoll.ctl(Epoll::Ctl:Del, client._cgiPipeReadFd);
	// 	_cgiPipeToClientFd.erase(client._cgiPipeReadFd);
	// }

	// // Close client socket
	// close(fd);
	// _epoll.ctl(Epoll::Ctl::Del, fd);
	// _clients.erase(fd); */

	std::cout << "Client " << data.fd << " Destructed\n";
}

void
ClientEvent::_in()
{
	switch (_state) {
		case State::READING_REQUEST:
			readRequest();
			break;
		case State::PARSING_REQUEST:
			parseRequest();
			break;
		case State::GENERATING_RESPONSE:
			generateResponse();
			break;
		case State::SENDING_RESPONSE:
			sendResponse();
			break;
/* 		// case State::EXECUTING_CGI:
		// 	CGIRequest();
		// 	break; */
		default:
			events = Epoll::Events::RdH;
	}
}

void
ClientEvent::readRequest()
{
	_requestBuffer += Socket::recv(data.fd);

	std::cout << "Client " << data.fd << " Request:\n" << _requestBuffer << "\n";

	if (_requestBuffer.find("\r\n\r\n") != std::string::npos) {
		_request += std::move(_requestBuffer);
		_state = State::PARSING_REQUEST;
		return;
	}

	if (_requestBuffer.empty())
		events = Epoll::Events::RdH;

}

void
ClientEvent::parseRequest()
{
	try {
		// call HttpRequestParser

		_state = State::GENERATING_RESPONSE;
	} catch (const std::exception& e) {
		LOGGER(log("Parse error: " + std::string(e.what())));
		_response.setStatus(400);
		_response.setContentType("text/html");
		_response.setBody("<html><body><h1>400 Bad Request</h1></body></html");
		_state = State::SENDING_RESPONSE;
	}
}

void
ClientEvent::generateResponse()
{
	std::istringstream stream(std::move(_requestBuffer));
	std::string method, path, version;
	stream >> method >> path >> version;

	if (CGI::isCgiRequest(path)) {
		// _state = State::EXECUTING_CGI;
		_state = State::SENDING_RESPONSE;
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
				_response.setBody(ErrorPages::getErrorPage(404));
			} else {
				_response.setStatus(200);
				_response.setContentType("text/html");
				_response.setBody(indexContent);
			}
		} else {
			// Return 404 error
			_response.setStatus(404);
			_response.setContentType("text/html");
			_response.setBody(ErrorPages::getErrorPage(404));
		}
		_state = State::SENDING_RESPONSE;
	}
}

void
ClientEvent::sendResponse()
{
	std::string responseStr = _response.toString();
	send(data.fd, responseStr.c_str(), responseStr.length(), 0);

	_state = State::DONE;
}
