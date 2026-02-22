#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int fd, Config::Listener const &config)
	:	Event(fd, Epoll::Events::In | Epoll::Events::RdH)
	,	r_config(config)
{
	std::cout << "Client " << data.fd << " \e[34mConstructed\e[0m\n";
}

ClientEvent::~ClientEvent()
{
	std::cout << "Client " << data.fd << " \e[34mDestructed\e[0m\n";
}

void
ClientEvent::_in()
{
	ssize_t	recieved = Socket::recv(data.fd, _requestBuffer);

	if (recieved == 0)
		_signal = Signal::Close;

	if (_requestBuffer.find("\r\n\r\n") == std::string::npos)
		return;

	// TODO: if content-length given, wait to recv whole request body

	_request.parse(std::move(_requestBuffer)); // leaves buffer empty
	std::cout << "\n\n" << _request.toString() << "\n";


	// Process request, just don't block by waiting for IO like CGI
	/*
		method function table: GET, POST, DELETE
		CGI is GET, right?

		and like actually make the response if parsing fails too

		then signal write
	*/

	// placeholder: return the cgi output or index page
	{
		if (_request.getVersion() == "0.9")
			_response.setVersion("0.9");

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

		_signal = Signal::Write;
	}
}

void
ClientEvent::_out()
{
	ssize_t	sent = Socket::send(data.fd, _responseBuffer);

	if (sent == -1) {
		if (!(errno == EAGAIN || errno == EWOULDBLOCK))
			_signal = Signal::Close;
		return;
	}

	_responseBuffer.erase(0, sent);

	if (_responseBuffer.empty()) {
		std::cout << "Client " << data.fd << " \e[32mCompleted Request.\e[0m\n";
		_signal = Signal::Close;
	}
}
