#ifndef	CGI_HPP
# define CGI_HPP

#include <string>
#include <map>
#include <vector>

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

	std::string	execute(
		const std::string& path,
		const std::string& method,
		const std::string& query,
		const std::string& body);

	std::string	getCgiExtension(const std::string& path);

	std::string	getCgiInterpreter(const std::string& extension);

	void		setupEnvironment(HttpRequest& request);

	std::string	executeScript(const std::string& interpreter,
		const std::string& scriptPath);
};

#endif