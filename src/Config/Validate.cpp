#include "Validate.hpp"
#include <algorithm>

Validate::Validate(const Config &config) : _config(config) {}

void
Validate::validateConfig()
{
	if (_config.servers.empty()){
		throw std::runtime_error("Config error: no servers defined");
	}
	for (size_t i = 0; i < _config.servers.size(); i++)
	{
		validateServer(_config.servers[i]);
		if (!_validated)
			throw std::runtime_error("Config error: something in server is not valid");
	}
}

void
Validate::validateServer(const Config::Server &server)
{
	validateHost(server.host);
	validatePort(server.port);
// check if exists:
		// 	std::string	name;
		// std::string	host;
		// std::string	root;
		// int			port = 0;
		// size_t		clientMaxBodySize;
	// check if they are valid.

	for (size_t i = 0; i < server.errorPages.size(); i++)
		validateErrorPage(server.errorPages[i]);

	for (size_t i = 0; i < server.locations.size(); i++)
		validateLocation(server.locations[i]);
}
void
Validate::validateHost(const std::string &host)
{
	std::stringstream	ss(host);
	std::string			part;
	int					count = 0;
	int					n = std::stoi(part);

	while (std::getline(ss, part, '.')){
		if (++count > 4) // 12.52.192.168.1 (not 4 parts)
			log("Host IP too big, format: 0.0.0.0");
		if (part.empty()) // 12..14.22 (empty part)
			log("Part can't be emtpy. Host IP format: 0.0.0.0");
		if (!std::all_of(part.begin(), part.end(), ::isdigit)) //abc.def.ghi.jkl (not digits)
			log("Host IP can only contain numbers");
		if (part.size() > 1 && part[0] == '0') // 01.2.3.4 (leading zeroes?)
		{
			int	pos = part.find_first_not_of('0');
			int	end = part.size();
			log(part + " is wrong, should be: " + part.substr(pos, end));
		}
		if (count != 4) // 192.168.1 (not 4 parts)
			log("Host IP too small, format: 0.0.0.0");
		if (n < 0 || n > 255) // 256.0.0.1 (part too big)
			log("All numbers should be between 0-255");
	}
}

void
Validate::validatePort(int port)
{
	if (port < 1 || port > 65535)
		log("Config error: port must be between 1 - 65535");
}

void
Validate::validateErrorPage(const Config::Server::Page &errorPage)
{
	validErrorCodes.count(errorPage.code);
	if (errorPage.path.empty())
		log("");
	// check if page.path exists
	// check if the file is not empty??
}

void
Validate::validateLocation(const Config::Server::Location &location)
{
	location.path;
	location.root;
	location.clientMaxBodySize;
	location.returnURL.code;
	location.returnURL.path;
	location.uploadDir;
	location.index;
	location.cgiEXT;
	location.cgiPath;
	for (int i = 0; i < location.allowedMethods.size(); i++)
	{
		if (location.allowedMethods[i] 
	}
	for (int i = 0; i < location.indexFiles.size(); i++)
	{
		location.indexFiles[i];
	}
	// // check :
	// 			std::string	path;
	// 		std::string	root;
	// 		size_t		clientMaxBodySize = 0;
	// 		Page		returnURL;
	// 		int			redirectStatus = 0;
	// 		bool		autoindex = false;
	// 		std::string	uploadDir;
	// 		std::string	index;
	// 		std::string	cgiEXT;
	// 		std::string	cgiPath;

	// 		std::vector<std::string>	allowedMethods;
	// 		std::vector<std::string>	indexFiles;
}





void
Validate::log(std::string const &message) {
	_log.push_back(message);
}

void
Validate::report()
{
	if (_log.empty())
		return ;

	LOGGER(startBlock("Config File Errors"));
	for (std::string message : _log)
		LOGGER(log(message));
	LOGGER(endBlock());

	throw std::runtime_error(std::string("Config File Errors detected, see \"") + Logger::FileName + "\" for detailed error messages");
}
