#ifndef CONFIG_HPP
# define CONFIG_HPP

// C
// C++
# include <string>
# include <vector>

struct	Config
{
	struct	Server {
		std::string	name;
		std::string	host;
		std::string	root;
		int			port;
		size_t		clientMaxBodySize;
	};

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

	Server					server;
	std::vector<Location>	locations;
	std::vector<ErrorPage>	errorPages;
};

#endif
