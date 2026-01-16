#include "Server.hpp"
#include "HttpResponse.hpp"
#include "ClientState.hpp"
#include "CgiHandler.hpp"
#include <sstream>
#include <map>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

std::atomic<bool> Server::_running(false);

Server::Server()
	:	_socket(_port)
	,	_epoll(_socket)
	,	_cgiHandler()
{
	signal(SIGINT, shutdown);
	signal(SIGTERM, shutdown); // should this happen in main() or ::run() instead? static _running doesn't work for multiple servers anyway
}

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
				else if (_cgiPipeToClientFd.find(event.data.fd) != cgiPipeToClientFd.end()) {
					// This is a CGI pipe FD with data available.
					readCgiOutput(event.data.fd);
				} else {
					existingClient(event.data.fd); // this should absolutely pass a Socket... but how do I make Event::Fd a Socket???
				}
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
	int clientFd = _socket.accept();

	Epoll::Event	event(_socket.accept());
	_epoll.ctl(Epoll::Ctl::Add, event);
	_clients[clientFd] = ClientState();

	std::cout << "New client connected | Client fd = " << clientFd << "\n";
}

void
Server::existingClient(int fd)
const {

	auto clientIt = _clients.find(fd);
	if (clientIt == _clients.end()) {
		close(fd);
		_epoll.ctl(Epoll::Ctl::Del, fd);
		return;
	}
	
	ClientState& client = clientIt->second;

	switch (client._currentState) {
		case ClientState::READING_REQUEST:
			readRequest(fd, client);
			break;
		case ClientState::PARSING_REQUEST:
			parseRequest(fd, client);
			break;
		case ClientState::GENERATING_RESPONSE:
			generateResponse(fd, client);
			break;
		case ClientState::EXECUTIING_CGI:
			executeCgi(fd, client);
			break;
		case ClientState::SENDING_RESPONSE:
			sendResponse(fd, client);
			break;
	}
}

void
Server::readRequest(int fd, ClientState& client)
const {
	char buffer[4096];
	ssize_t nBytes = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);

	if (nBytes > 0) {
		buffer[nBytes] = '\0'
		client.appendToRequestBuffer(std::string(buffer,nBytes));
		
		if (client.isRequestComplete()){
			client._currentState = ClientState::PARSING_REQUEST;
		}
	} else if (nBytes == 0) {
		std::cout << "Client " << fd << " disconnected\n";
		cleanupClient(fd, client);
	} else if (errno != EAGAIN && errno != EWOULDBLOACK) {
		return;
	} else {
		Logger::log("recv() error: " + std::string(strerror(errno)));
		cleanupClient(fd, client);
	}
}

void 
Server::parseRequest(int fd, ClientState& client)
const {
	try {
		// TODO: Call your teamate's HTTP parser
		// client._request = HttpReqeustParser::parse(client._requestBuffer);

		client._currentState - ClientState::GENERATING_RESPONSE;
	} catch (const std::execption& e) {
		Logger::log("Parse error: " + std::string(e.what()));
		client._response.setStatus(400);
		client._response.setContentType("text/html");
		client._response.setBody("<html><body><h1>400 Bad Request</h1></body></html");
		client._responseBuffer = client.response.toString();
		client._currentState - ClientState::SENDING_RESPONSE;
	}
}

// NOTE: This code is probably not suitable for handling cgi properly. or generating a response.
void server::generateResponse(int fd, ClientState& client)
const {

	(void)fd;

	// Check if this is a CGI request by checking request buffer
	bool isCgiRequest = _cgiHandler.isCgiRequest(client._requestBuffer);

	if (isCgiRequest) {
		client._currentState = ClientState::EXECUTING_CGI;
		return;
	}

	client._response.setStatus(200);
	client._response.setContentType("text/html");
	client._response.setBody("<html><body><h1>Hello From Webserve! (CGI response)</h1></body></html>");

	client._responseBuffer = client._response.toString();
	client._currentState = ClientState::SENDING_RESPOSNE;
}

void 
Server::executeCgi(int fd, ClientState& client)
const {
	// Onlsy start CGI once per client
	if (client._cgiPid != -1)
		return;
	
	try {
		// TODO: Extract path, maethod, query, bodu from client,_request
		// FOr now, use placehold values
		CgiHandler::CgiProcess process = _cgiHandler.executeAsync(
			"/cgi", // path 
			"GET",	// method
			"",		// query
			""		// body
		);

		// Store CGI process info
		client._cgiPid = process.processId;
		client.cgiPipeReadFd = process.ouotputPipeFd;

		EpollEvent pipeEvent(process.outputPipefd);
		_epoll.ctl(Epoll::Ctl::Add, pipeEvent);

		_cgiPipeToClientFd[process.outputPipeFd] = fd;

		std::cout << "CGI process started | pid = "<< process.processId << ", pipe = " << process.outputPipeFd << "\n";
	} catch (const std::exception& e) {
		// CGI execution fialed
		Logger::log("CGI error: " + std:string(e.what()));
		client._response.setStatus(500);
		client._response.setContentType("text/html");
		client._response.setBody("<html><body><h1>500 CGI Error</h1></body></html>");
		client._responseBuffer = client._response.toString();
		client._currentState = ClientState::SENDING_RESPOSNE;
		
	}
}
void
Server::zombieClient(int fd)
const {
	close(fd);
	_epoll.ctl(Epoll::Ctl::Del, fd);
}

