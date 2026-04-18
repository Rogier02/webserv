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

		if (!seen.insert(location.first).second)
			log("Config error: duplicate location " + location.first);
		validateLocation(location.second, location.first);
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
			log(host + " is incorrect: part can't be empty. Host IP format: 0.0.0.0");
		if (!std::all_of(part.begin(), part.end(), ::isdigit)) //abc.def.ghi.jkl (not digits)
			log(host + " is incorrect: can only contain numbers");
		if (part.size() > 1 && part[0] == '0') // 01.2.3.4 (leading zeroes?)
		{
			int	pos = part.find_first_not_of('0');
			int	end = part.size();
			log(host + " is incorrect: " + part + " should be: " + part.substr(pos, end));
		}
		if (n < 0 || n > 255) // 256.0.0.1 (part too big)
		log(host + " is incorrect: all numbers should be between 0-255");
	}
	if (count > 4) // 12.52.192.168.1 (not 4 parts)
		log(host + " is incorrect: too many parts, format: 0.0.0.0");
	else if (count < 4) // 192.168.1 (not 4 parts)
		log(host + " is incorrect: not enough parts, format: 0.0.0.0");
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
			log("Error page path is empty!");
		else if (!IO::exists(errorPage.second))
			log(errorPage.second + " file doesn't exist.");
	}
}

void
Validate::validateLocation(const Config::Listener::Location &location, const std::string &name)
{
	ensureDirectory(location.root);
	ensureDirectory(location.returnURL);
	if (!location.uploadDir.empty())
		ensureDirectory(location.root + location.uploadDir);
	ensureDirectory(location.cgiPath);
	if ((location.clientMaxBodySize > 104857600 || location.clientMaxBodySize < 1) && location.clientMaxBodySize != 0)
		log("client_max_body_size of " + std::to_string(location.clientMaxBodySize) + " is too big. Max: 100m");
	if (!IO::exists("." + location.root + location.index))
		log(location.index + " file doesn't exist in directory: " + location.root);

	const std::unordered_set<std::string> valid = {
		"GET", "POST", "DELETE"
	};

	for (const std::string& method : location.allowedMethods)
		if (!valid.contains(method))
			log("In location: " + name + " [" + method + "] is NOT an allowed method!");
}

void
Validate::ensureDirectory(const std::string& path)
{
	if (!path.empty())
		std::filesystem::create_directories("." + path);
}

void
Validate::log(std::string const &message) {
	_log.push_back(message);
}

void
Validate::report()
{
	if (_log.empty()){
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