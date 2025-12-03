#include "HttpRequest.hpp"
#include "TokenStream.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

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

// void
// HttpRequest::parseHttpRequest(std::string request){
// 	std::string line;
// 	int	lineNbr = 0;
// 	while (std::getline(request, line)){
// 		lineNbr++;
// 		if (lineNbr == 1)
// 			parseRequestLine(line);
// 		else
// 			headers.push_back(parseHeader(line));


// 		// httpTokens.push_back()
// 	}
// }

// void
// HttpRequest::parseRequestLine(std::string line){

// 	std::vector<std::string> parts = split(line, ' ');

// 	method = parseMethod(parts[0]);
// 	target = parts[1];
// 	size_t delimiter = parts[1].find('?');
// 	path = parts[1].substr(0, delimiter);
// 	query = parts[1].substr(delimiter);
// 	version = parseVersion(parts[2]);
// }

// int
// parseMethod(std::string method){
// 	while (allowedMethods){
// 		if (method == allowedMethods)
// 			return (method);
// 	}
// 	else
// 		LOG("[HTTP Request Error] invalid method: " << method << "\n");
// }

// HttpRequest::Version
// parseVersion(std::string version){
// 	Version	httpVersion;

// 	std::string::size_type slash = version.find('/');
// 	std::string::size_type dot = version.find('.');

// 	version.httpMajorVersion = std::stoi(version.substr(slash + 1, 1));
// 	version.httpMinorVersion = std::stoi(version.substr(dot + 1, 1));

// 	return (httpVersion);
// }

// HttpRequest::Header
// parseHeader(std::string line){
// 	HttpRequest::Header	header;
// 	size_t	colonPos = line.find(':');

// 	header.headerName = line.substr(0, colonPos);
// 	header.headerValue = line.substr(colonPos);
// 	return (header);
// }