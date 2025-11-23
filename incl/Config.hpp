#ifndef CONFIG_HPP
# define CONFIG_HPP

// C++
# include <string>
# include <vector>
# include <stdexcept>

struct	Token
{
	std::string	text;
	int			lineNbr;
};

struct	Config
{
	struct	Server {
		struct Location	{
			// copied from github, I have no idea what these mean
			std::string					path;
			std::string					root;
			size_t						clientMaxBodySize;
			std::string					returnURL;
			int							redirectStatus;
			bool						autoindex;
			std::string					uploadDir;
			std::string					index;
			std::string					cgiEXT;
			std::string					cgiPath;
			std::vector<std::string>	allowedMethods;
			std::vector<std::string>	indexFiles;
		};

		struct ErrorPage {
			int			code;
			std::string	path;
		};

		std::string	name;
		std::string	host;
		std::string	root;
		int			port;
		size_t		clientMaxBodySize;
		std::vector<Location>	locations;
		std::vector<ErrorPage>	errorPages;
	};

	std::vector<Server>	_servers;

	class Error: public std::exception {
		public:
			Error(std::string const &message = "No apparent reason...");
			const char	*what() const throw();
		private:
			std::string	_message;
	};

	Config();
	Config(std::string file);
	Config &operator=(Config &&other);
	
};

#endif
