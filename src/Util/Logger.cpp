#include "Logger.hpp"

Logger &
Logger::get()
{
	static Logger	instance;
	return (instance);
}

Logger::Logger()
	:	_logFile(FileName, std::ios::out | std::ios::trunc)
{
	if (!_logFile.is_open())
		throw std::ios_base::failure("Logger failed to create Log file");
	header();
}

Logger::~Logger() {
	_logFile.close();
}

void
Logger::log(Level level, std::string const &message, const char *file, int line)
{
	const char *PREFIX[] = {
		"ERROR",
		"INFO",
		"WARNING",
		"DEBUG",
	};

	put(PREFIX[level], '\t');
	put(message, ' ');
	if (level == Error || level == Debug)
		put(std::string("(") + file + ":" + std::to_string(line) + ")");
	putendl();
}

void Logger::put(std::string const &str, char c) {
	_logFile << str << c;
}

void Logger::putendl(std::string const &str) {
	_logFile << str << "\n";
}

void
Logger::header()
{
	time_t	timer = time(NULL);

	_logFile
		<< "Webserv Log\n"
		<< std::put_time(std::localtime(&timer), "%F %R %Z (UTC%z)\n\n");
}
