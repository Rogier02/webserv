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
Logger::startBlock(std::string const &blockName, std::string const &blockHeader)
{
	_currentBlock = blockName;

	const size_t	headerWidth	= std::max(_currentBlock.size(), blockHeader.size()) + 8;

	log(std::string(headerWidth, BlockSeparator));
	log("\tStart of Block:\t" + _currentBlock);
	if (!blockHeader.empty())
		log("\t\t" + blockHeader);
	log(std::string(headerWidth, BlockSeparator));
}

void
Logger::endBlock()
{
	const size_t	headerWidth = _currentBlock.size() + 8;

	log(std::string(headerWidth, BlockSeparator));
	log("\tEnd of Block:\t" + _currentBlock);
	log(std::string(headerWidth, BlockSeparator));

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
