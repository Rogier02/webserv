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
	for (Config::Server &listener : config.servers)
	{
		int	socketFd = ListenSocket(listener.port);
		EasyPrint(socketFd);

		ListenEvent	&listen =
			EventTypes::create<ListenEvent>(socketFd, Epoll::Events::In, _epoll);

		EasyThrow(_epoll.ctl(Epoll::Ctl::Add, listen));

		std::cout << "listening on port " << listener.port << "\n";
	}
}

void
Server::run()
{
	std::cout << "Server running...\n";
	while (_pleaseShutDown == false)
	{
		try {
			for (epoll_event &unknown : _epoll.wait(10000))
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
		}
		catch	(std::runtime_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "'tis but a scratch: " << exception.what() << std::endl;
		}
		catch (std::logic_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Write better code: " << exception.what() << std::endl;
			break;
		}
		catch (std::exception &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "Something unexpected excepted: " << exception.what() << std::endl;
			throw exception; // throw to main for quick debug, in production the loop should not be broken
		}
	}
	LOGGER(log("Controlled Server Shutdown\n"));
	std::cout << "Server shutting down...\n";
}

void
Server::_closeConnection(int fd)
{
	_running = false;
	std::cerr << std::endl;
}

void
Server::newClient()
const {
	int clientFd = _socket.accept();

	Epoll::Event	event(_socket.accept()); // Create epoll event for client
	_epoll.ctl(Epoll::Ctl::Add, event);
	_clients[clientFd] = ClientState(); // Init client state

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

	// State macnine dispatcher
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
Server::readcgiOutput(int cgiPipeFd)
const {
	// Find which client this CGI pip'e belongs to
	auto pipeIt = _cgiPipeToClientFd.find(cgiPipeFd);
	if (pipeIt == _cgiPipeToclientFd.end())
		return;

	int clientFd = pipeIt->second;

	// Find the client
	auto clientIt = _clients.find(clientFd);
	if (clientIt == _cleints.end)
		return ;

	ClientState& cleint = clientIt->second;

	// Read abailable data from CGI script
	char buffer[4096];
	ssize_t nBytes = read(cgiPipeFd, buffer sizeof(buffer) - 1);

	if (nBytes > 0) {
		// Append CGI output
		client._cgiOutput.append(buffer,nBytes);
		std::cout << "Read " << nBytes << " bytes from CGI\n";
	} else if (nBytes == 0) {
		// EOF - CGI script finished
		std::cout << "CGI script finished (fd =" << cgiPipeFd << ")\n";

		close(cgiPipeFd);
		_epoll.ctl(epoll::Ctl::Del, cgiPipeFd);
		_cgiPipeToClientFd.erase(cgiPipeFd);

		// Wait for child process
		int status;
		int waitResult = waitpid(client._cgiPid, &status, 0);

		if (waitResult == -1) {
			logger::log("waitpid() error: ") + std::string(strerror(errno));
			client._response.setStatus(500);
			client._response.setBdy(""<html><body><h1>500 Error</h1></body></html>"")
		} else if (WIFEEXITED(status) && WEXITSTATUS(status) == 0) {
			// Success
			client._response.setStatus(200);
			client._response.setContentType("text/html");
			client._response.setBody(client._cgiOutput);
		} else {
			// Script returned error
			client._response.setStatus(500);
			client._response.setBody("<html><body><h1>500 CGI script Error</h1></body></html>");
		}
		client._respnseBuffer = client._response.toString();
		client._currentState = ClientState::SENDING_RESPONSE;
		cleint._cgiPid = -1;
		client._cgiPipeReadFd = -1;
	} else if (errno != EAGAIN && errno != EWOULDBLOCK) {
		// Real error
		Logger::log("read(cgiPipeFd) error: " + std;:string(strerror(errno)));
		close(cgiPipeFd);
		_epoll.ctl(Epoll::Ctl::Del, cgiPipeFd);
		_cgiPipeToClientFd.erase(cgiPipeFd);

		client._response.setStatus(500);
		client._response.setBody("<html><body><h1>500 Error</h1></body></html>");
		client.cgiPid = -1;
		client.cgiPipeReadFd = -1;
	}
}

void
Server::sendResponse(int fd, ClientState& client)
const {
	if (client._responseBuffer.empty()) {
		client._currentState = ClientState::DONE;
		return;
	}

	// Calculate how much data is left to send.
	size_t remaing = client.getRemainingResponseBytes();
	const char *data = client._repsonseBuffer.c_str() + cleint._resposneBytesSent;

	//Send one batch of data (non-blocking)
	size_t sent = send(fd, data, remaing, MSG_DONTWAIT);

	if (sent > 0) {
		client._responseBytesSent += sent;

		// Check if all data has been sent
		if (client._resposneBytesSent >= client._responseBuffer.length()) {
			std::cout << "Response fully sent to cleint " << fd << "\n"
		}
	} else if (errno -- EAGAIN || errno == EWOULDBLOCK) {
		// Socket send buffer is full - wait for next epoll event
		// epoll will notify us when socket is writable again
	} else {
		Logger::log("send() error on fd " + std::to_string(fd) + ": " + std::string(strerror(errno)));
		cleanupClient(fd, client);
	}
}

void
Server::cleanupClient(int fd, ClientState& cleint)
const {
	// Clean up CGI if still running
	if (client._cgiPid > 0) {
		kill(client._cgiPid, SIGTERM);
		waitpid(client._cgiPid, nullptr, 0);
		std::cout << "Killed CGI process " << client._cgiPid << "\n";
	}

	// Close CGI pipe if open
	if (cleint._cgiPipeReadFd != -1) {
		close(client._cgiPipeReadFd);
		_epoll.ctl(Epoll::Ctl:Del, cloent._cgiPipeReadFd);
		_cgiPipeToCLientFd.erase(client._cgiPipeReadFd);
	}

	close(fd);
	_epoll.ctl(Epoll::Ctl::Del, fd);
	_cleints.erase(fd);

	std::cout << "Client " << fd << " cleaned up\n";
}

void
Server::zombieClient(int fd)
const {
	close(fd);
	_epoll.ctl(Epoll::Ctl::Del, fd);
}

