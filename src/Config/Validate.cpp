#include "Validate.hpp"
#include <algorithm>
#include <filesystem>
#include <unordered_set>

Validate::Validate(const Config &config) : _config(config) {
	validateConfig();
}

void
Validate::validateConfig()
{
	if (_config.servers.empty()){
		throw std::runtime_error("Config error: no servers defined");
	}
	for (size_t i = 0; i < _config.servers.size(); i++)
	{
		validateServer(_config.servers[i]);
		// if (!_validated)
		// 	log("Config error: something in server is not valid");
	}
	report();
}

void
Validate::validateServer(const Config::Server &server)
{
	if (server.name.empty())
		log("Config error: server name is empty");
	validateHost(server.host);
	validatePort(server.port);

	for (size_t i = 0; i < server.errorPages.size(); i++)
		validateErrorPage(server.errorPages[i]);

	for (size_t i = 0; i < server.locations.size(); i++){
		std::unordered_set<std::string>	seen;

		if (!seen.insert(server.locations[i].path).second) //duplicate locations		startup	❌
			log("Config error: duplicate location");
		validateLocation(server.locations[i]);
	}
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
	if (!validErrorCodes.count(errorPage.code))
		log("Config error: " + std::to_string(errorPage.code) + " does not belong to the list of valid errorPages.");
	// 	error page missing		startup	✅ (warning in terminal)
	if (errorPage.path.empty())
		std::cout << "Error page path is empty!" << std::endl;
	else if (!fileExists(errorPage.path))
		std::cout << errorPage.path << " file doesn't exist." << std::endl;
}

bool
Validate::isValidMethod(std::string method)
{
	// invalid HTTP methods	startup	❌;
	return (method == "GET" || method == "POST" || method == "DELETE");
}

void
Validate::validateLocation(const Config::Server::Location &location)
{
	// location root missing	startup	✅ (warning in terminal)
	if (!location.path.empty() && !directoryExists(location.path))
		std::cout << location.path << " directory doesn't exist." << std::endl;
	// location root missing	startup	✅ (warning in terminal)
	if (!location.root.empty() && !directoryExists(location.root))
		std::cout << location.root << " directory doesn't exist." << std::endl;
	if ((location.clientMaxBodySize > 104857600 || location.clientMaxBodySize < 1) && location.clientMaxBodySize != 0)
		log("client_max_body_size of " + std::to_string(location.clientMaxBodySize) + " is too big. Max: 100m");
	if (!location.returnURL.path.empty() && !directoryExists(location.returnURL.path))
		log(location.returnURL.path + " directory doesn't exist.");
	if (!location.uploadDir.empty() && !directoryExists(location.uploadDir))
		log(location.uploadDir + " directory doesn't exist.");
	// location.index; >> dit is nog helemaal unknown.
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
			log(location.indexFiles[i] + " file doesn't exist.");
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
	if (_log.empty()){
		// _validated = true;
		return ;
	}

	LOGGER(startBlock("Config File Errors"));
	for (std::string message : _log)
		LOGGER(log(message));
	LOGGER(endBlock());

	throw std::runtime_error(std::string("Config File Errors detected, see \"") + Logger::FileName + "\" for detailed error messages");
}


void	iAmHere(std::string string)
{
	std::cout << "==========================================\n";
	std::cout << "=========== This is where I am ===========\n";
	std::cout << "==========================================\n";
	std::cout << "I am in: " << string << std::endl;
}