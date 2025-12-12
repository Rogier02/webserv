#include "Server.hpp"
#include "HttpResponse.hpp"
#include "CgiHandler.hpp"
#include "ErrorPageHandler.hpp"
#include <sstream>
#include <errno.h>

std::atomic<bool> Server::_running(false);

Server::Server()
	:	_socket(_port)
	,	_epoll(_socket)
	,	_cgiHandler()
	,	_errorPageHandler()
{
	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown); // should this happen in main() or ::run() instead? static _running doesn't work for multiple servers anyway
}

/* Server::Server(std::vector<Config::Server> const &config)
	:	_config(config)
	,	_socket()
	,	_epoll(_socket)
{
	(void)_config;

	sockaddr_in	serverAddress{};
	serverAddress.sin_family		= AF_INET;
	serverAddress.sin_port			= htons(_port);
	serverAddress.sin_addr.s_addr	= INADDR_ANY;

	if (bind(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("bind()");

	if (listen(_socket, 5) == -1)
		throw std::runtime_error("listen()");

	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown);
} */

void
Server::run()
const {
	_running = true;
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_running)
	{
		try {
			for (Epoll::Event &event : _epoll.wait())
			{
				if (event.isWeird())
					zombieClient(event.data.fd);
				else if (event.data.fd == _socket)
					newClient();
				else
					existingClient(event.data.fd);// this should absolutely pass a Socket... but how do I make Event::Fd a Socket???
			}
		} catch	(std::runtime_error &restartRequired) {
			Logger::log(restartRequired.what());
			std::cerr << restartRequired.what() << std::endl;
		} catch (std::exception &unknownException) {
			// log unknown and throw to main? or just don't catch and let Logger class catch somehow?
			throw unknownException;
		}
	}
	std::cout << "Server shutting down...\n";
}

void
Server::shutdown(int)
{
	_running = false;
	std::cerr << std::endl;
}

void
Server::newClient()
const {
	Epoll::Event	event(_socket.accept());

	_epoll.ctl(Epoll::Ctl::Add, event);
	std::cout << "New client connected.\n";
}

// Rogier's additions Nov 16 - start
void
Server::existingClient(int fd)
const {
	std::string	request;
	char buffer[1024];

	// Step 1: Read complete HTTP request from client
	// Must wait for all headers (ends with \r\n\r\n)
	while (true){
		// Read available data (non-blocking)
		ssize_t nBytes = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);

		if (nBytes > 0) {
			//Received data - add to request buffer
			buffer[nBytes] = '\0';
			request.append(buffer, nBytes);

			//Check if we have a complete headers (ends with \r\n\r\n)
			if (request.find("\r\n\r\n") != std::string::npos) {
				break; // Complete request received
			}
		} else if (nBytes == 0) {
			// Client disconnected
			std::cout << "client disconnected.\n";
			_epoll.ctl(Epoll::Ctl::Del, fd);
			close(fd);
			return ;
		} else {
			// EAGAIN/EWOULDBLOCK means no more data available right now
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				if (!request.empty()) {
					break; // we have datal, process it
				}
				return; // No data yet wait for next event
			} else {
				// Real error
				std::cout << "Client disconnected: " << strerror(errno) << std::endl;
				_epoll.ctl(Epoll::Ctl::Del, fd);
				close(fd);
				return;
			}

		}
	}

	// Validate we have request data
	if (request.empty()) {
		return;
	}

	// Parse request (tempcode will be repaced with HttpRequest parser)

	std::cout << "Client Request:\n" << request << std::endl;
	// TODO: Implatement Raw request parser -- request(rawRequest);

	
	// Temporary simple parsing ( REPLACE: )
	std::istringstream stream(request);
	std::string method, path, version;
	stream >> method >> path >> version;

	// Http response object
	HttpResponse response(200);

	// Check if CGI request
	if (_cgiHandler.isCgiRequest(path)) {
		// Execute CGI script
		std::string cgiOutput = _cgiHandler.execute(path, method, "", "");
		response.setStatus(200);
		response.setContentType("text/html");
		response.setBody(cgiOutput);
	} else {
		// Non-CGI request (static files)
		// Temporary: Return 404 for any path that's not / or /cgi.py
		if (path == "/") {
			std::string indexContent = readFile("./webpages/index.html");
			if (!indexContent.empty()) {
				response.setStatus(200);
				response.setContentType("text/html");
				response.setBody(indexContent);
			} else {
				response.setStatus(404);
				response.setContentType("text/html");
				response.setBody(_errorPageHandler.getErrorPage(404));
			}
		} else {
			// Return 404 error
			response.setStatus(404);
			response.setContentType("text/html");
			response.setBody(_errorPageHandler.getErrorPage(404));
		}	
	}

	// Send response
	std::string responseStr = response.toString();
	send(fd, responseStr.c_str(), responseStr.length(), 0);


	std::cout << responseStr;
	// // Close connections (HTTP/1.0 style for now)
	// close(fd);
	_epoll.ctl(Epoll::Ctl::Del, fd);
}
// End 

void
Server::zombieClient(int fd)
const {
	close(fd);
	_epoll.ctl(Epoll::Ctl::Del, fd);
}

std::string
Server::readFile(const std::string& filePath) const
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		return ("");
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return (buffer.str());
}
