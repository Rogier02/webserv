#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int socketFd, Epoll::Events events)
	:	Event(socketFd, events)
{}

void
ClientEvent::_in()
const {
	std::string	request;

	while (request.find("\r\n\r\n") == std::string::npos)
	{
		request += Socket::recv(data.fd);
		if (request.empty())
			throw CloseConnection(data.fd); // what is this behaviour for? should it close on empty recv or empty request?
	}
	std::cout << "Client Request:\n" << request << std::endl;

	HttpResponse	response(200);
	response.setContentType("text/html");
	response.setBody(
		"<html>\n"
			"<head><title>Webserv</title></head>\n"
			"<body>\n"
				"<h1>Hello from Webserv!</h1>\n"
				"<p>Your request was received successfully.</p>\n"
			"</body>\n"
		"</html>\n"
	);

	Socket::send(data.fd, response.toString());
}
