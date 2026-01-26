#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include <string>
# include <map>

# include <fstream>
# include <sstream>

namespace ErrorPages
{
	static std::map<int, std::string>	_pages = {
		{000, "Unknown Error"},
		{400, "Bad Request"},
		{404, "Not Found"},
		{500, "Internal Server Error"},
		{502, "Bad Gateway"},
		{503, "Service Unavailable"},
	};

	std::string getBody(int code);
	std::string getTitle(int code);
};

#endif