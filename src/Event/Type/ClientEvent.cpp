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
	std::cout << "Client " << data.fd << " Request:\n" << request << "\n";

	// HttpResponse	response(200);
	// response.setContentType("text/html");
	// response.setBody(
	// 	"<html>\n"
	// 		"<head><title>Webserv</title></head>\n"
	// 		"<body>\n"
	// 			"<h1>Hello from Webserv!</h1>\n"
	// 			"<p>Your request was received successfully.</p>\n"
	// 		"</body>\n"
	// 	"</html>\n"
	// );

	// Socket::send(data.fd, response.toString());

	// Temporary simple parsing ( REPLACE: )
	std::istringstream stream(request);
	std::string method, path, version;
	stream >> method >> path >> version;

	// Http response object
	HttpResponse	response;

	// Check if CGI request
	if (CGI::isCgiRequest(path)) {
		// Execute CGI script
		std::string cgiOutput = CGI::execute(path, method, "", "");
		response.setStatus(200);
		response.setContentType("text/html");
		response.setBody(cgiOutput);
	} else {
		// Non-CGI request (static files)
		// Temporary: Return 404 for any path that's not / or /cgi.py
		if (path == "/") {
			std::string indexContent = readFile("./defaultPages/index.html");
			if (!indexContent.empty()) {
				response.setStatus(200);
				response.setContentType("text/html");
				response.setBody(indexContent);
			} else {
				response.setStatus(404);
				response.setContentType("text/html");
				response.setBody(ErrorPages::getErrorPage(404));
			}
		} else {
			// Return 404 error
			response.setStatus(404);
			response.setContentType("text/html");
			response.setBody(ErrorPages::getErrorPage(404));
		}
	}

	// Send response
	std::string responseStr = response.toString();
	send(data.fd, responseStr.c_str(), responseStr.length(), 0);

	// std::cout << "Client " << data.fd << " Response:\n" << responseStr << "\n";
}
