#include "HttpRequest.hpp"
#include "TokenStream.hpp"
#include <sstream>

/*
-- HTTP1.0 request example --
GET /index.html HTTP/1.0\r\n
Host: www.example.com\r\n
User-Agent: Mozilla/5.0\r\n
Accept: text/html\r\n
\r\n
*/

//1. read first line --> split into method, path, version
//2. read header lines until empty line
//3. if content-length exists --> read that many bytes for the body

void
HttpRequest::parseHttpRequest(int clientFd){

	char		buffer[4096];
	std::string	request;
	std::string line;
	while (true)
	{
		ssize_t	n	=	recv(clientFd, buffer, 4096, 0);
		request.append(buffer);
		if (request.find("\r\n\r\n") != std::string::npos)
			break;
	}

	std::istringstream	requestStream(request);
	while (std::getline(requestStream, line))
	{
		std::stringstream	stream(line);
		std::string			word;
		while (stream >> word){
			method = parseMethod(word[0]);
			target = word[1];
			version = paseVersion(word[2]);


		}
	}


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

HttpVersion
HttpRequest::parseVersion(std::string version){
	Version	httpVersion;

	std::string::size_type slash = version.find('/');
	std::string::size_type dot = version.find('.');

	version.httpMajorVersion = std::stoi(version.substr(slash + 1, 1));
	version.httpMinorVersion = std::stoi(version.substr(dot + 1, 1));

	return (httpVersion);
}
