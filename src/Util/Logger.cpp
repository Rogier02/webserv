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

void Logger::log(std::string const &message) {
	_logFile << message << "\n";
}

void
Logger::startBlock(std::string const &blockName, std::string const &blockDescription)
{
	_currentBlock = blockName;

	const std::string	start	= "Start of Block:";
	const size_t		width	= std::max(_currentBlock.size(), blockDescription.size())
								+ ((start.size()/4) * 4) + 12;

	log(std::string(width, BlockSeparator));
	log("\t" + start + "\t" + _currentBlock);
	if (!blockDescription.empty())
		log("\t\t" + blockDescription);
	log(std::string(width, BlockSeparator));
}

void
Logger::endBlock()
{
	const std::string	end		= "End of Block:";
	const size_t		width	= _currentBlock.size()
								+ ((end.size()/4) * 4) + 12;

	log(std::string(width, BlockSeparator));
	log("\t" + end + "\t" + _currentBlock);
	log(std::string(width, BlockSeparator) + "\n");

	_currentBlock.clear();
}

void
Logger::header()
{
	time_t	timer = time(NULL);

	_logFile
		<< "Webserv Log\n"
		<< std::put_time(std::localtime(&timer), "%F %R %Z (UTC%z)\n\n");
}
