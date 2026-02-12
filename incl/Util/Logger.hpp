#ifndef LOGGER_HPP
# define LOGGER_HPP

// C++
# include <string>
# include <fstream>
# include <ctime>
# include <iomanip>

class	Logger
{
	public:
		static Logger	&get();// trying to make singleton usage stand out, might have to update Style guide for it?

	public:
		static constexpr const char	*FileName = "Log.txt";

	private:
		static constexpr char		BlockSeparator = '=';

	private:
		Logger();
		Logger(Logger const &) = delete;
		Logger(Logger &&) = delete;
		~Logger();

	public:
		void	log(std::string const &message);

		void	startBlock(std::string const &blockName, std::string const &blockHeader = "");
		void	endBlock();

	private:
		std::ofstream	_logFile;
		std::string		_currentBlock;

	private:
		void	header();
};

# define LOGGER(call) Logger::get().call

#endif
