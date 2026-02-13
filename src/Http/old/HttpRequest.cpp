#include "HttpRequest.hpp"
#include <vector>
#include <fstream>
#include <iostream>

/*
-- HTTP1.0 request example --
GET /index.html HTTP/1.0
Host: www.example.com
User-Agent: Mozilla/5.0
Accept: text/html
*/

//1. read first line --> split into method, path, version
//2. read header lines until empty line
//3. if content-length exists --> read that many bytes for the body

void
HttpRequest::parseHttpRequest(){

	std::string request;
	std::string line = getline(request);
	std::vector<std::string> parts = split(line, ' ');

	method = parseMethod(parts[0]);
	target = parts[1];
	version = paseVersion(parts[2]);


	std::vector<Token> httpTokens;
	TokenStream httpTS(httpTokens);

	httpTS.current
}

std::string
HttpRequest::parseMethod(std::string method){
	while (allowedMethods){
		if (method == allowedMethods)
			return (method);
	}
	else if (method)
	else
		LOG("[HTTP Request Error] invalid method: " << method << "\n");
}

Version
HttpRequest::parseVersion(std::string version){
	Version	httpVersion;

	std::string::size_type slash = version.find('/');
	std::string::size_type dot = version.find('.');

	version.httpMajorVersion = std::stoi(version.substr(slash + 1, 1));
	version.httpMinorVersion = std::stoi(version.substr(dot + 1, 1));

	return (httpVersion);
}
