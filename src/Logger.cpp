#include "Logger.hpp"

Logger::Logger()
	:	_logFile("failLog.txt", std::ios::out | std::ios::trunc)
{
	if (!_logFile.is_open())
		throw std::ios_base::failure("Logger failed to create Log file");
	header();
}

Logger::~Logger() {
	_logFile.close();
}

Logger &
Logger::get()
{
	static Logger	instance;

	return (instance);
}

void	Logger::log(std::string const &message) {
	get()._logFile << message << "\n";
}

void	Logger::header() {
	_logFile << "Webserv Log\n" << "time: " << time(NULL) << "\n\n";
}
