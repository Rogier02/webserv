#ifndef LOGGER_HPP
# define LOGGER_HPP

// C++
# include <string>
# include <fstream>

class	Logger
{
	private:
		Logger();
		Logger(Logger const &other) = delete;
		Logger(Logger &&other) = delete;
		~Logger();
	public:
		static void	log(std::string const &message);
	public:
		static Logger &get();
	private:
		std::ofstream	_logFile;
		void			header();
};

#endif
