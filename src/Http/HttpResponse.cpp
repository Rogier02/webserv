#include "HttpResponse.hpp"
#include <sstream>
#include <ctime>

HttpResponse::HttpResponse(int statusCode) : _statusCode(statusCode)
{
	switch (statusCode) {
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

	// Default Headers;
	_headers["Server"] = "webserv/1.0";
	_headers["Connection"] = "close";

	// Add current headers
	std::time_t now = std::time(nullptr);
	char dateBuffer[100];
	std::strftime(dateBuffer, sizeof(dateBuffer), "%a, %d %b %Y %H:%M:%SGMT", std::gmtime(&now));
	_headers["Date"] = std::string(dateBuffer);
}

void HttpResponse::setStatus(int code)
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