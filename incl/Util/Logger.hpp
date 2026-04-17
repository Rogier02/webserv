#ifndef LOGGER_HPP
# define LOGGER_HPP

// C++
# include <iostream>
# include <string>
# include <fstream>
# include <ctime>
# include <iomanip>

// # define DEBUG

class	Logger
{
	public:
		static Logger	&get();

	public:
		static constexpr const char	*FileName = "Log.txt";

		enum	Level : ::size_t {
			Error = 0,
			Info,
			Warning,
			Debug,
		};

	private:
		Logger();
		Logger(Logger const &) = delete;
		Logger(Logger &&) = delete;
		~Logger();

	private:
		std::ofstream	_logFile;

	public:
		void	log(Level level, std::string const &message, const char *file, int line);

	private:
		void	put(std::string const &str, char c = 0);
		void	putendl(std::string const &str = "");

		void	header();
};

# define LOG(level, message) Logger::get().log(Logger::Level::level, message, __FILE__, __LINE__)

#endif
