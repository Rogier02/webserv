#include "Validate.hpp"
#include <algorithm>
#include <filesystem>

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
			log("Config error: something in server is not valid");
	}
	report();
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
	
	while (std::getline(ss, part, '.')){
		count++;
		int	n = std::stoi(part);

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
		if (n < 0 || n > 255) // 256.0.0.1 (part too big)
		log("All numbers should be between 0-255");
	}
	if (count > 4) // 12.52.192.168.1 (not 4 parts)
		log("Host IP too big, format: 0.0.0.0");
	if (count != 4) // 192.168.1 (not 4 parts)
		log("Host IP too small, format: 0.0.0.0");
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
		log("Error page path is empty!");
	else if (!fileExists(errorPage.path))
		log(errorPage.path + " file doesn't exist.");
	// check if the file is not empty?? maybe don't check for this in validate.
}

bool
isValidMethod(std::string method)
{
	return (method == "GET" || method == "POST" || method == "DELETE");
}

void
Validate::validateLocation(const Config::Server::Location &location)
{
	if (!location.path.empty() && !directoryExists(location.path))
		log(location.path + " directory doesn't exist.");
	if (!location.root.empty() && !directoryExists(location.root))
		log(location.root + " directory doesn't exist.");
	// location.clientMaxBodySize;
	// location.returnURL.code;
	if (!location.returnURL.path.empty() && !directoryExists(location.returnURL.path))
		log(location.returnURL.path + " directory doesn't exist.");
	if (!location.uploadDir.empty() && !directoryExists(location.uploadDir))
		log(location.uploadDir + " directory doesn't exist.");
	// location.index;
	// location.cgiEXT;
	if (!location.cgiPath.empty() && !directoryExists(location.cgiPath))
		log(location.cgiPath + " directory doesn't exist.");
	// location.redirectStatus;
	for (unsigned long i = 0; i < location.allowedMethods.size(); i++)
	{
		if (!isValidMethod(location.allowedMethods[i]))
			log(location.allowedMethods[i] + " is an NOT an allowed method!");
	}
	for (unsigned long i = 0; i < location.indexFiles.size(); i++)
	{
		if (!fileExists(location.indexFiles[i]))
			log(location.indexFiles[i] + " doesn't exist.");
	}
}

bool
Validate::directoryExists(const std::string& path)
{
	if (std::filesystem::path(path).is_absolute())
	{
		std::string	currentPath = std::filesystem::current_path();
		std::string fullPath = currentPath + path;
		return (std::filesystem::is_directory(fullPath));
	}
	return (std::filesystem::is_directory(path));
}

bool
Validate::fileExists(const std::string& path)
{
	if (std::filesystem::path(path).is_absolute())
	{
		std::string	currentPath = std::filesystem::current_path();
		std::string fullPath = currentPath + path;
		return (std::filesystem::is_regular_file(fullPath));
	}
	return (std::filesystem::is_regular_file(path));
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


void	iAmHere()
{
	std::cout << "==========================================\n";
	std::cout << "=========== This is where I am ===========\n";
	std::cout << "==========================================" << std::endl;
}