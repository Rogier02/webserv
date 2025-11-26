#include "Config.hpp"
#include "TokenStream.hpp"
#include "parsing.hpp"
#include "log.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Config::Server
parseServer(TokenStream &ts){
	Config::Server server;
	ts.expect("{");
	while (!ts.atEnd() && ts.current().text != "}"){
		std::string current = ts.takeToken();
		if (current == "server_name"){
			server.name = ts.takeToken();
			ts.checkSemicolon();
		}
		else if (current == "listen"){
			std::string	hostPort = ts.takeToken();
			size_t colonPos = hostPort.find(':');
			server.host = parseHost(hostPort, colonPos);
			server.port = parsePort(hostPort, colonPos);
			ts.checkSemicolon();
		}
		else if (current == "client_max_body_size")
			server.clientMaxBodySize = parseClientMaxBodySize(ts);
		else if (current == "root"){
			server.root = ts.takeToken();
			ts.checkSemicolon();
		}
		else if (current == "error_page")
			server.errorPages.push_back(parseErrorPage(ts));
		else if (current == "location")
			server.locations.push_back(parseLocation(ts));
		else {
			LOG("[Config Error] \t\"" << ts.current().text << "\" -> Unknown directive on line " << ts.current().lineNbr << ": " << ts.getLine());
			ts.setIndex(ts.lastTokenOnLine() + 1); //check if this works correctly
		}
	}
	ts.expect("}");
	return (server);
}

std::string
parseHost(std::string hostPort, size_t colonPos){
	std::string	host = hostPort.substr(0, colonPos);

	return (host);
}

int
parsePort(std::string hostPort, size_t colonPos){
	int			port = std::stoi(hostPort.substr(colonPos + 1));

	return (port);
}

Config::Page
parseErrorPage(TokenStream &ts){
	Config::Page errorPage;
	errorPage.code = std::stoi(ts.takeToken());
	errorPage.path = ts.takeToken();
	ts.checkSemicolon();
	return (errorPage);
}