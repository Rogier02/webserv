#include "Http_v1_0.hpp"

namespace Http {
	Message::Message(
		std::string const &version)
		:	_version(version)
	{}

	void
	Message::writeHeaders(
		std::string		&dest,
		HeaderMap const	&headerMap
	) {
		for (std::pair<const std::string, std::string> const &header : headerMap)
			dest += header.first + ": " + header.second + CRLF;
	}

/*
POST /api/login HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)
Accept: application/json, text/plain
Accept-Language: en-US,en;q=0.9
Accept-Encoding: gzip, deflate, br
Content-Type: application/json
Content-Length: 52
Connection: keep-alive
Origin: https://example.com
Referer: https://example.com/login
Cookie: sessionId=abc123xyz; theme=dark

{
  "username": "john_doe",
  "password": "mypassword"
}
*/

//1. read first line --> split into method, path, version
//2. read header lines until empty line
//3. if content-length exists --> read that many bytes for the body
	Request::Request(
		std::string const &request
	) {
		std::istringstream stream(request);
		std::string line;
		std::getline(stream, line);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		// Parse request line
		size_t a = line.find(' ');
		size_t b = line.find(' ', a + 1);

		_method = parseMethod(line.substr(0, a));
		_URI = line.substr(a + 1, b - a - 1);
		_version = line.substr(b + 1);
		// Parse request headers
		while (std::getline(stream, line))
		{
			if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

			if (line.empty())
				break;

			size_t colon = line.find(':');
			std::string	key = line.substr(0, colon);
			std::string value = line.substr(colon + 1);
			_requestHeaders.insert(std::make_pair(key, value));
		}
		// Parse body
		std::map<std::string, std::string>::iterator it = _requestHeaders.find("Content-Length");
		if (it != _requestHeaders.end())
		{
			int	contentLength = std::stoi(it->second.c_str());
			char buffer[10000];
			stream.read(buffer, contentLength);
			_entityBody.assign(buffer, stream.gcount());
		}
		// parse request line for method, URI, version, otherwise v0.9
	}

	std::string
	Request::parseMethod(
		std::string method
	) {
		if (method == "GET" || method == "POST" || method == "DELETE")
			return (method);
		else
			return (NULL); //throw error maybe here??
	}

	Response::Response(
		std::string const	&version,
		u_int8_t			statusCode
	)	:	Message(version)
		,	_statusCode(statusCode)
		,	_reasonPhrase(StatusCodes[_statusCode])
	{
		_entityHeaders["content-type"] = "text/html";
		_entityHeaders["content-length"] = "0";
	}

	std::string
	Request::toString()
	const {
		std::string	requestLine;
		std::string	headers;

		requestLine = _method + SP + _URI + SP + _version + CRLF;

		writeHeaders(headers, _generalHeaders);
		writeHeaders(headers, _requestHeaders);
		writeHeaders(headers, _entityHeaders);

		return (requestLine + headers + CRLF + _entityBody);
	}

	std::string
	Response::toString()
	const {
		std::string	statusLine;
		std::string	headers;

		statusLine = "HTTP/" + _version + SP + std::to_string(_statusCode) + SP + _reasonPhrase + CRLF;

		writeHeaders(headers, _generalHeaders);
		writeHeaders(headers, _responseHeaders);
		writeHeaders(headers, _entityHeaders);

		return (statusLine + headers + CRLF + _entityBody);
	}

	int
	Response::setEntityBody(
		std::string const &content
	) {
		_entityBody = content;

		_entityHeaders["content-length"] = std::to_string(content.length());
		// other entity headers

		return (0);
	}

	std::string const &
	Request::getVersion()
	const {
		return (_version);
	}

	std::string const &
	Request::getMethod()
	const {
		return (_method);
	}

	std::string
	Request::getURI()
	const {
		return (_URI);
	}

	std::string const &
	Request::getEntityBody()
	const {
		return (_entityBody);
	}

	std::string
	Request::getQueryString()
	const {
		std::string	queryString = _URI.substr((_URI.find_first_of("?") + 1), _URI.size());
		return (queryString);
	}

	std::string
	Request::getScriptName()
	const {
		std::string	scriptName = _URI.substr(0, _URI.find_first_of("?"));
		return (scriptName);
	}

	std::string
	Request::getHost(std::string const &key)
	const {
		std::string	host = getGeneralHeaderValue("Host");
		if (key == "Port")
		{
			std::string	port = host.substr(host.find_first_of(":") + 1, host.size());
			return (port);
		}
		else
		{
			std::string	address = host.substr(0, host.find_first_of(":"));
			return (address);
		}
	}

	std::string const	&
	Request::getRequestHeaderValue(std::string const &key)
	const {
		return (_requestHeaders.at(key));
	}

	std::string const	&
	Message::getGeneralHeaderValue(std::string const &key)
	const {
		return (_generalHeaders.at(key));
	}

	std::string const	&
	Message::getEntityHeaderValue(std::string const &key)
	const {
		return (_entityHeaders.at(key));
	}

	//Rogier

	Http::HeaderMap const &
	Request::getRequestHeaders()
	const {
		return (_requestHeaders);
	}

	Http::HeaderMap const &
	Request::getGeneralHeaders()
	const {
		return (_generalHeaders);
	}

	Http::HeaderMap const &
	Request::getEntityHeaders()
	const {
		return (_entityHeaders);
	}

	int
	Response::setResponseHeaderValue(std::string const &key, std::string const &value) {
		_responseHeaders[key] = value;
		return (0);
	}

	int
	Response::setStatuscode(u_int8_t statusCode) {
		_statusCode = statusCode;
		if (StatusCodes.find(statusCode) != StatusCodes.end())
			_reasonPhrase = StatusCodes[statusCode];
		return (0);
	}
}

