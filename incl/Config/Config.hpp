#ifndef CONFIG_HPP
# define CONFIG_HPP

// C++
# include <string>
# include <vector>
# include <map>
# include <iostream>

struct	Config //should probably be Server class
{

	struct	Listener
	{
		struct Location
		{
			std::string	root; // if directive is missing, enter Listener root + location path
			std::string	index;
			std::string	allowedMethods;
			std::string	uploadDir;
			size_t		clientMaxBodySize = 0;
			u_int16_t	redirectStatus = 0;
			std::string	returnURL;
			std::string	cgiEXT;
			std::string	cgiPath;
			bool		autoindex = false;
		};

		std::string	name;
		std::string	root;
		std::string	host;
		int			port = 8080;
		size_t		clientMaxBodySize;

		std::map<u_int16_t, std::string>	errorPages;
		std::map<std::string, Location>		locations;
	};

	std::vector<Listener>	listeners;
};

std::ostream	&operator<<(std::ostream &os, Config const &config);

#endif
