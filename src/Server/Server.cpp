#include "Server.hpp"
#include "HttpResponse.hpp"
#include <sstream>

Server::Server()
	:	_listenSocket(_port)
{
	Event	event(EpollEvents::In, _listenSocket);
	_epoll.ctl(Epoll::Ctl::Add, event);
}

void
Server::run()
{
	std::cout << "Server running... (listening on port " << _port << ")\n";
	while (_pleaseShutDown == false)
	{
		try {
			for (epoll_event &unknown : _epoll.wait())
			{
				if (unknown.data.fd == _listenSocket)
					_addClient();
				else if (unknown.events & (EpollEvents::Err | EpollEvents::Hup))
					_delClient(unknown.data.fd);
				else try {
					Event	*event = EventTypes::get(unknown.data.fd);
					event->handle();
				} catch (int closing) {
					_delClient(closing);
				}
			}
		}
		catch	(std::runtime_error &exception) {
			LOGGER(log(exception.what()));
			std::cerr << "'tis but a scratch: " << exception.what() << std::endl;
		}
		catch (EventTypes::NotRegistered &exception) {
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
Server::_addClient()
{
	Event	client(EpollEvents::In, _listenSocket.accept());

	EventTypes::specify<ClientEvent>(client);

	_epoll.ctl(Epoll::Ctl::Add, client);
	std::cout << "Client " << client.data.fd << " connected.\n";
}

void
Server::_delClient(int fd)
{
	_epoll.ctl(Epoll::Ctl::Del, fd);
	EasyThrow(close(fd));
	std::cout << "Client " << fd << " disconnected.\n";
}

/*
// Rogier's additions Nov 16 - start
//	Simon's additions Nov 17 - start
//	kaolo lange functie maat
// 	P.S. this function logic was moved to ClientEvent._in();
// 	End
void
Server::existingClient(int fd)
const {
	std::string	request;
	char buffer[1024];

	while (true){
		ssize_t nBytes = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);

		if (nBytes > 0) {
			buffer[nBytes] = '\0';
			request.append(buffer, nBytes);

			//Check if we have a complete headers (ends with \r\n\r\n)
			if (request.find("\r\n\r\n") != std::string::npos) {
				break;
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
	if (request.empty()) {
		return;
	}

		std::cout << "Client Request:\n" << request << std::endl;



		// Http response
		HttpResponse response(200);
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

		// Send response
		std::string responseStr = response.toString();
		send(fd, responseStr.c_str(), responseStr.length(), 0);

		// Close connections (HTTP/1.0 style for now)
		close(fd);
		_epoll.ctl(Epoll::Ctl::Del, fd);
}
// End
*/