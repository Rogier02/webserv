#include <string>
#include <iostream>
#include "Config.hpp"
#include <string>
#include <iostream>
#include <fstream>

Config::Config(){}


// Config &
// Config::operator=(Config &&other)
// {
// 	if (this != &other)
// 	{

// 	}
// 	return (*this);
// }

// Config::Error::Error(std::string const &message)
// 	:	_message(message)
// {}

// const char *
// Config::Error::what()
// const throw() {
// 	return (_message.c_str());
// }



std::ostream& operator<<(std::ostream& os, const Config::Server::Location& loc) {
	os << "\tLocation {\n"
	   << "\t  path: " << loc.path << "\n"
	   << "\t  root: " << loc.root << "\n"
	   << "\t  clientMaxBodySize: " << loc.clientMaxBodySize << "\n"
	   << "\t  returnURL: " << loc.returnURL << "\n"
	   << "\t  redirectStatus: " << loc.redirectStatus << "\n"
	   << "\t  autoindex: " << (loc.autoindex ? "true" : "false") << "\n"
	   << "\t  uploadDir: " << loc.uploadDir << "\n"
	   << "\t  index: " << loc.index << "\n"
	   << "\t  cgiEXT: " << loc.cgiEXT << "\n"
	   << "\t  cgiPath: " << loc.cgiPath << "\n";

	os << "\t  allowedMethods: [";
	for (size_t i = 0; i < loc.allowedMethods.size(); i++)
		os << loc.allowedMethods[i] << (i + 1 < loc.allowedMethods.size() ? ", " : "");
	os << "]\n";

	os << "\t  indexFiles: [";
	for (size_t i = 0; i < loc.indexFiles.size(); i++)
		os << loc.indexFiles[i] << (i + 1 < loc.indexFiles.size() ? ", " : "");
	os << "]\n";

	return os << "\t}\n";
}

std::ostream& operator<<(std::ostream& os, const Config::Server::ErrorPage& err) {
	return os << "\tErrorPage { code: " << err.code
	          << ", path: " << err.path << " }\n";
}

std::ostream& operator<<(std::ostream& os, const Config::Server& s) {
	os << "Server {\n"
	   << "  name: " << s.name << "\n"
	   << "  host: " << s.host << "\n"
	   << "  root: " << s.root << "\n"
	   << "  port: " << s.port << "\n"
	   << "  clientMaxBodySize: " << s.clientMaxBodySize << "\n";

	os << "  Locations:\n";
	for (size_t i = 0; i < s.locations.size(); i++)
		os << s.locations[i];

	os << "  ErrorPages:\n";
	for (size_t i = 0; i < s.errorPages.size(); i++)
		os << s.errorPages[i];

	return os << "}\n";
}

std::ostream& operator<<(std::ostream& os, const Config& c) {
	os << "Config:\n";
	for (size_t i = 0; i < c.servers.size(); i++)
		os << c.servers[i] << "\n";
	return os;
}

void
Config::printConfig(){
	std::cout << *this << std::endl;
}

