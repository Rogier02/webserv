#ifndef	CGI_HPP
# define CGI_HPP

#include <string>
#include <map>
#include <vector>
#include <HttpRequest.hpp>

class CGI
{
	private:
			
		const std::string BinDirectory = "./src/cgi-bin/";
		const std::map<std::string, std::string> SupportedExtensions = {
			{".py", "/usr/bin/python3"},
			//{".sh", "/bin/bash"}
			//{".php", "/usr/bin/php-cgi"},
			// {".pl", "/usr/bin/perl"}
		};

	public:
		CGI() = default;
		~CGI() = default;

		bool isCgiRequest(const std::string& path) const;
		std::string execute(HttpRequest& request);
	
	private:

		std::string getCgiExtension(const std::string& path) const;
		std::string getCgiInterpreter(const std::string& extension) const;
		char 		**setupEnvironment(HttpRequest& request);
		std::string executescript(
			const std;:string& interpreter,
			const std::string& scriptPath,
			const std::string& requestBody,
			char **envp);

		std::string	parseCgiResponse(const std::string& rawOutput) const;
};

#endif