#ifndef CONFIG_HPP
# define CONFIG_HPP

// C++
# include <string>
# include <vector>
# include <iostream>

struct	Config
{
	struct	Server
	{
		struct Page
		{
			int			code = 0;
			std::string	path;
		};

		struct Location
		{
			std::string	path;
			std::string	root;
			size_t		clientMaxBodySize = 0;
			Page		returnURL;
			int			redirectStatus = 0;
			bool		autoindex = false;
			std::string	uploadDir;
			std::string	index;
			std::string	cgiEXT;
			std::string	cgiPath;

			std::vector<std::string>	allowedMethods;
			std::vector<std::string>	indexFiles;
		};

		std::string	name;
		std::string	host;
		std::string	root;
		int			port = 0;
		size_t		clientMaxBodySize;

		std::vector<Page>		errorPages;
		std::vector<Location>	locations;
	};

	std::vector<Server>	servers;
};

std::ostream	&operator<<(std::ostream &os, Config const &config);

#endif
