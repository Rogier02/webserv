#ifndef CONFIG_HPP
# define CONFIG_HPP

// C++
# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <sstream>
# include <set>

struct	Config
{

	struct	Listener
	{
		struct Location
		{
			std::string				root;
			std::string				index;
			std::string				uploadDir;
			::size_t				clientMaxBodySize = 0;
			u_int16_t				redirectStatus = 0;
			std::string				redirect;
			std::string				cgiEXT;
			std::string				cgiPath;
			bool					autoindex = false;
			std::set<std::string>	allowedMethods;
		};

		std::string	name;
		std::string	root;
		std::string	host;
		int			port = 8080;
		::size_t	clientMaxBodySize = 0;

		std::map<u_int16_t, std::string>	errorPages;
		std::map<std::string, Location>		locations;
	};

	std::vector<Listener>	listeners;

	std::string	toString() const;
};

#endif
