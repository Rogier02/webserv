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


/* 	Client IN should:
		- read the request (in chunks)
		- parse the request? maybe server can hold request and response instead

	Then the Server should:
		- fulfill the request (GET, POST, CGI?)
		- make up the response

	Then somehow Client OUT should trigger......
		- and Socket::send(response);
*/


void
ClientEvent::_in()
{
	ssize_t	recieved = Socket::recv(data.fd, _requestBuffer);

	if (recieved == 0)
		throw CloseConnection(data.fd);

	if (_requestBuffer.find("\r\n\r\n") == std::string::npos)
		return;

	// if content-length given, wait to recv whole request body (maybe start time-out clock?)

	_request.parse(std::move(_requestBuffer)); // leaves buffer empty
	std::cout << "\n\n" << _request.toString() << "\n";

	// MIDDLE request processing placeholder

	if (_request.getVersion() != "0.9")
		_response.setVersion("1.0");

	if(CGI::isCgiRequest(_request.getURI()))
	{
		_response.setEntityBody(CGI::execute(_request));
	}
	else
	{
		std::string indexContent = readFile("./www/index.html");
		if (indexContent.empty()) {
			_response.err(404);
		} else {
			_response.setEntityBody(indexContent);
		}
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
