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
	version = parseVersion(parts[2]);


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

