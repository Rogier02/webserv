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
	if (_config.listeners.empty()){
		throw std::runtime_error("Config error: no servers defined");
	}
	for (const Config::Listener &server : _config.listeners)
	{
		validateServer(server);
		// if (!_validated)
		// 	log("Config error: something in server is not valid");
	}
	report();
}

void
Validate::validateServer(const Config::Listener &server)
{
	if (server.name.empty())
		log("Config error: server name is empty");
	validateHost(server.host);
	validatePort(server.port);

	validateErrorPages(server);

	for (std::pair<const std::string, Config::Listener::Location> const &location : server.locations)
	{
		std::unordered_set<std::string>	seen;

		if (seen.insert(location.first).second) //duplicate locations		startup	❌
			log("Config error: duplicate location");
		validateLocation(location.second);
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
	else if (count < 4) // 192.168.1 (not 4 parts)
		log("Host IP too small, format: 0.0.0.0");
}

void
Validate::validatePort(int port)
{
	if (port < 1 || port > 65535)
		log("Config error: port must be between 1 - 65535");
}

void
Validate::validateErrorPages(const Config::Listener &server)
{
	for (std::pair<const u_int16_t, std::string> const &errorPage : server.errorPages)
	{
		if (!Http::Statuses.contains(errorPage.first))
			log("Config error: " + std::to_string(errorPage.first) + " does not belong to the list of valid errorPages.");
		if (errorPage.second.empty())
			std::cout << "Error page path is empty!" << std::endl;
		else if (!IO::exists(errorPage.second))
			std::cout << errorPage.second << " file doesn't exist." << std::endl;
	}
}

bool
Validate::isValidMethod(std::string method)
{
	// invalid HTTP methods	startup	❌;
	return (method == "GET" || method == "POST" || method == "DELETE");
}

void
Validate::validateLocation(const Config::Listener::Location &location)
{
	// location root missing	startup	✅ (warning in terminal)
	if (!location.root.empty() && !directoryExists(location.root))
		std::cout << location.root << " directory doesn't exist." << std::endl;
	if ((location.clientMaxBodySize > 104857600 || location.clientMaxBodySize < 1) && location.clientMaxBodySize != 0)
		log("client_max_body_size of " + std::to_string(location.clientMaxBodySize) + " is too big. Max: 100m");
	if (!location.returnURL.empty() && !directoryExists(location.returnURL))
		log(location.returnURL + " directory doesn't exist.");
	if (!location.uploadDir.empty() && !directoryExists(location.uploadDir))
		log(location.uploadDir + " directory doesn't exist.");
	// location.index; >> dit is nog helemaal unknown.
	// location.cgiEXT;
	if (!location.cgiPath.empty() && !directoryExists(location.cgiPath))
		log(location.cgiPath + " directory doesn't exist.");
	// location.redirectStatus;
	// for (unsigned long i = 0; i < location.allowedMethods.size(); i++) // dis string now
	// {
	// 	if (!isValidMethod(location.allowedMethods[i]))
	// 		log(location.allowedMethods[i] + " is an NOT an allowed method!");
	// }
	if (!IO::exists(location.index))
		log(location.index + " file doesn't exist.");
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

	for (std::string message : _log)
		LOG(Info, message);

	throw std::runtime_error(std::string("Config File Errors detected, see \"") + Logger::FileName + "\" for detailed error messages");
}


void	iAmHere(std::string string)
{
	std::cout << "==========================================\n";
	std::cout << "=========== This is where I am ===========\n";
	std::cout << "==========================================\n";
	std::cout << "I am in: " << string << std::endl;
}