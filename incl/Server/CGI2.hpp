#ifndef	CGI2_HPP
# define CGI2_HPP

// C
# include <sys/wait.h>
// C++
# include <string>
// webserv
# include "Config.hpp"
# include "ClientEvent.hpp"

namespace	CGI2
{
	const std::map<std::string, std::string> SupportedExtensions = {
		{".py", "/usr/bin/python3"},
		{".sh", "/bin/bash"},
		//{".php", "/usr/bin/php-cgi"},
		// {".pl", "/usr/bin/perl"},
	};
}

#endif