#ifndef	CGI_HPP
# define CGI_HPP

// C++
#include <string>
#include <map>
#include <vector>

// webserv
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

namespace	CGI
{
	const std::string BinDirectory = "./src/cgi-bin/";

	const std::map<std::string, std::string> SupportedExtensions = {
		{".py", "/usr/bin/python3"},
		{".sh", "/bin/bash"}
		//{".php", "/usr/bin/php-cgi"},
		// {".pl", "/usr/bin/perl"}
	};

	bool		isCgiRequest(const std::string& path);
	std::string execute(Http::Request& request);

	std::string	getCgiExtension(const std::string& path);

	std::string	getCgiInterpreter(const std::string& extension);

	char 		**setupEnvironment(Http::Request& request);

	std::string	executeScript(
		const std::string& interpreter,
		const std::string& scriptPath,
		const std::string& requestBody,
		char **env);

	std::string	parseCgiResponse(const std::string& rawOutput);
};

#endif