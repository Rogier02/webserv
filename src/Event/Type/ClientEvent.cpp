#include "ClientEvent.hpp"

void
ClientEvent::_in()
const {
	std::string	request = Socket::recv(data.fd);
	if (request.empty()) {
		throw data.fd;
	} else {
		std::cout << "Client Request:\n" << request << std::endl;
	}
}

void
ClientEvent::_out()
const {}
