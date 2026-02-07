#ifndef	CGI_HPP
# define CGI_HPP

#include <string>
#include <map>
#include <vector>
#include <HttpRequest.hpp>

class CGI
{
	// public :
	// 	enum state {
	// 		INACTIVE,
	// 		READ,
	// 		WRITE
	// 	};

	private:

		//state state;

		const HttpRequest& request;

		std::string buffer;
		std::string Response;

		pid_t	pid;
		int		cgi_stdin;
		int		cgi_stdout;


	private:
		
		const std::string BinDirectory = "./src/cgi-bin/";
		const std::map<std::string, std::string> SupportedExtensions = {
			{".py", "/usr/bin/python3"},
			//{".sh", "/bin/bash"}
			//{".php", "/usr/bin/php-cgi"},
			// {".pl", "/usr/bin/perl"}
		};

	public :

		bool		isCgiRequest(const std::string& path);
		bool		isCgiRequestSupported(const std::string& path);

		void		captureOutput(std::string &buffer);

		std::string	configureCgiResponse();
		std::string	execute(HttpRequest& request);
		
		//std::string	getCgiInterpreter(const std::string& extension);

		char 		**setupEnvironment(HttpRequest& request);
		char 		**envMapToArray()

};

#endif