#include "ParseHttpRequest.hpp"


#define HTTPREQUEST 2

ParseHttpRequest::ParseHttpRequest(std::istream& stream, int streamType, Config &config)
	:	_ts(stream, streamType)
	,	_config(config)
{}

/*
-- HTTP1.0 request example --
GET /index.cgi?=jlkdsjfjr=h HTTP/1.0\r\n
Host: www.example.com\r\n
User-Agent: Mozilla/5.0\r\n
Accept: text/html\r\n
\r\n
*/

//1. read first line --> split into method, path, version
//2. read header lines until empty line
//3. if content-length exists --> read that many bytes for the body

HttpRequest
ParseHttpRequest::httpRequest()
{
	HttpRequest	httpRequest;


			method = parseMethod(word[0]);
			target = word[1];
			version = parseVersion(word[2]);

}

std::string
ParseHttpRequest::parseMethod(std::string method){
	while (_config.servers.locations[i].allowMethods){
		if (method == allowedMethods)
			return (method);
	}
	else if (method)
	else
		LOG("[HTTP Request Error] invalid method: " << method << "\n");
}

HttpRequest::HttpVersion
ParseHttpRequest::parseVersion(std::string version)
{
	HttpRequest::HttpVersion	httpVersion;

	std::string::size_type slash = version.find('/');
	std::string::size_type dot = version.find('.');

	httpVersion.major = std::stoi(version.substr(slash + 1, 1));
	httpVersion.minor = std::stoi(version.substr(dot + 1, 1));

	return (httpVersion);
}