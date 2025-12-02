#ifndef LOGGER_HPP
# define LOGGER_HPP

// C++
# include <string>
# include <fstream>

class	Logger
{
	public:
		static Logger	&get();// trying to make singleton usage stand out, might have to update Style guide for it?

	private:
		static constexpr const char	*FileName = "Log.txt";
		static constexpr char		BlockSeparator = '=';

	private:
		Logger();
		Logger(Logger const &other) = delete;
		Logger(Logger &&other) = delete;
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
