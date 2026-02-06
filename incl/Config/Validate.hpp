#ifndef VALIDATE_HPP
#define VALIDATE_HPP

#include "Config.hpp"

class Validate
{
private:
	const Config				&_config;
	std::vector<std::string>	_log;
	bool						_validated;

public:
	Validate() = delete;
	Validate(Validate const &other) = delete;
	Validate(Validate &&other) = delete;
	Validate(const Config &config);
	~Validate() = default;
	
	void	validateConfig();
	void	validateServer(const Config::Server &server);
	void	validateHost(const std::string &host);
	void	validatePort(int port);
	void	validateErrorPage(const Config::Server::Page &errorPage);
	bool	isValidMethod(std::string method);
	void	validateLocation(const Config::Server::Location &location);
	bool	directoryExists(const std::string& path);
	bool	fileExists(const std::string& path);

private:
	void	log(std::string const &message);
	void	report();
};

void	iAmHere();

#endif