#include "HttpResponse.hpp"
#include <sstream>
#include <ctime>

HttpResponse::HttpResponse(int statusCode)
	:	_statusCode(statusCode)
{
	// if (errorPageHandler.isError(statuscode))
	// 	_reasonPhrase = errorPageHandler.getError(statuscode);

	setStatus(statusCode);

	// Default Headers;
	_headers["Server"] = "webserv/1.0";
	_headers["Connection"] = "close";

	// Add current headers
	std::time_t now = std::time(nullptr);
	char dateBuffer[100];
	std::strftime(dateBuffer, sizeof(dateBuffer), "%a, %d %b %Y %H:%M:%SGMT", std::gmtime(&now));
	_headers["Date"] = std::string(dateBuffer);
}

void 
HttpResponse::setStatus(int code)
{
	_statusCode = code;
	switch (code) {
		case 200:
			_reasonPhrase = "OK";
			break;
		case 404:
			_reasonPhrase = "Not Found";
			break;
		case 500:
			_reasonPhrase = "Internal Server Error";
			break;
		default:
			_reasonPhrase = "Unknown";
			break;
	}
}

std::string 
HttpResponse::toString() 
const 
{
	std::stringstream response;

	//Status line 
	response << "HTTP/1.1 " << _statusCode << " " << _reasonPhrase << "\r\n";

	// Headers
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
		response << it->first << ": " << it->second << "\r\n";
	}

	// Seperate body from headers
	response << "\r\n";

	// Body
	response << _body;

	return response.str();
}

void 
HttpResponse::setHeader(const std::string& key, const std::string& value)
{
	_headers[key] = value;
}

void 
HttpResponse::setBody(const std::string& body)
{
	_body = body;
	_headers["Content-Length"] = std::to_string(body.length());
}

void 
HttpResponse::setContentType(const std::string& type)
{
	_headers["Content-Type"] = type;
}