#ifndef VALIDATE_HPP
#define VALIDATE_HPP

#include "Config.hpp"
#include "Logger.hpp"
#include <unordered_set>

class Validate
{
private:
	const Config				&_config;
	std::vector<std::string>	_log;
	// bool						_validated;

public:
	Validate() = delete;
	Validate(Validate const &other) = delete;
	Validate(Validate &&other) = delete;
	Validate(const Config &config);
	~Validate() = default;

	void	validateConfig();
	void	validateServer(const Config::Listener &server);
	void	validateHost(const std::string &host);
	void	validatePort(int port);
	void	validateErrorPage(const Config::Listener::Page &errorPage);
	bool	isValidMethod(std::string method);
	void	validateLocation(const Config::Listener::Location &location);
	bool	directoryExists(const std::string& path);
	bool	fileExists(const std::string& path);

private:
	void	log(std::string const &message);
	void	report();
};

void	iAmHere(std::string string);

// These are all the http valid errorCodes I could find. If we have less, we can make this list way smaller
static const std::unordered_set<int> validErrorCodes = {
	// 4xx Client Errors
	400,401,402,403,404,405,406,407,408,409,
	410,411,412,413,414,415,416,417,
	418,421,422,423,424,425,426,428,
	429,431,451,

	// 5xx Server Errors
	500,501,502,503,504,505,506,507,
	508,510,511
};

#endif