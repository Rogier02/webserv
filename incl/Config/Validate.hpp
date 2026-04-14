#ifndef VALIDATE_HPP
# define VALIDATE_HPP

// C++
# include <unordered_set>
// webserv
# include "Config.hpp"
# include "Logger.hpp"
# include "IO.hpp"
# include "HttpMessage.hpp"

class Validate
{
private:
	const Config				&_config;
	std::vector<std::string>	_log;

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
	void	validateErrorPages(const Config::Listener &server);
	bool	isValidMethod(std::string method);
	void	validateLocation(const Config::Listener::Location &location);
	bool	directoryExists(const std::string& path);

private:
	void	log(std::string const &message);
	void	report();
};


#endif