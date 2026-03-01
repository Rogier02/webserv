#include "HttpRequest.hpp"
#include <iostream>

# include "EasyPrint.hpp"

namespace Http {
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

	int
	Request::setEntityBody(std::string const &requestEntity)
	{
		const std::size_t	entityLength	= requestEntity.length();
		std::size_t			contentLength	= 0;

		if (_entityHeaders.contains("content-length"))
			contentLength = std::stoul(_entityHeaders.at("content-length"));

		if (entityLength > contentLength)
		{
			EasyPrint(entityLength - contentLength);
			return (-1);// bad request
		}

		if (entityLength < contentLength)
			return (0);// recv more

		_entityBody = requestEntity;
		return (1);// success
	}

	int
	Request::parseHead(std::string const &request)
	{
		std::istringstream stream(request);
		std::string line;

		getlineCRLF(stream, line);
		if (parseRequestLine(line) == -1)
			return (-1);
		if (parseHeaders(stream, line) == -1)
			return (-1);

		if (parseEntityBody(stream) == -1)
			return (-1);

		if (validateParseRequest() == -1)
			return (-1);

		return (0);
	}

	int
	Request::parseRequestLine(std::string const &line)
	{
		size_t sp1 = line.find(' ');

		if (sp1 == std::string::npos)
			return (-1);

		_method = line.substr(0, sp1);

		size_t sp2 = line.find(' ', sp1 + 1);

		if (sp2 == std::string::npos)
		{
			if (_method != "GET")
				return (-1); // Give error that HTTP/0.9 can only use GET method
			_URI = line.substr(sp1 + 1);
			_version = "HTTP/0.9";
		}
		else
		{
			if (line.find(' ', sp2 + 1) != std::string::npos)
				return (-1);
			_URI = line.substr(sp1 + 1, sp2 - sp1 - 1);
			_version = line.substr(sp2 + 1);
		}

		return (0);
	}

	int
	Request::parseHeaders(std::istream &stream, std::string &line)
	{
		while (getlineCRLF(stream, line) == 0)
		{
			if (line.empty())
				return (0);

			size_t colon = line.find(':');
			if (colon == std::string::npos)
				return (-1);
			std::string	key = line.substr(0, colon);
			for (int i = 0; i < key.size(); i++)
				key[i] = std::tolower((unsigned char)key[i]);
			std::string value = line.substr(colon + 1);
			std::map<std::string, HeaderHandler>::const_iterator it = HeaderHandlers.find(key);
			if (it == HeaderHandlers.end())
				continue;
			it->second(value);
		}
		return (0);
	}

	int
	Request::parseEntityBody(std::istream &stream)
	{
		std::map<std::string, std::string>::iterator it = _entityHeaders.find("content-length");

		if (it == _entityHeaders.end())
			return (0);

		size_t	contentLength = std::stoul(it->second);
		_entityBody.resize(contentLength);
		stream.read(_entityBody.data(), contentLength); //data returns pointer to where entityBody is stored

		if (stream.gcount() != contentLength)
			return (-1);
		return (0);
	}

	// Validation parse request //
	int
	Request::validateParseRequest()
	{
		if (_method != "GET"
		||	_method != "POST"
		||	_method != "DELETE"
		||	_method != "HEAD")
			return (-1);
		if (!validateHTTPVersion())
			return (-1);
		if (!validateURI())
			return (-1);
		if (!validateHeaders())
			return (-1);
		return (0);
	}

	int
	Request::validateHTTPVersion()
	{
		if (_version.empty())
			return (-1);
		if (_version.find("HTTP/") == std::string::npos)
			return (-1);

		size_t	dot = _version.find(".");
		if (dot == 5 || _version.size() <= dot + 5) // returns -1 if: HTTP/.9 or HTTP/1.
			return (-1);

		std::string	major = _version.substr(5, dot);
		std::string	minor = _version.substr(dot + 1, _version.size());
		if (!isAllDigits(major) || !isAllDigits(minor))
			return (-1);

		if ((std::stoi(major) >= 1 && std::stoi(minor) > 0) || (std::stoi(major) == 0 && std::stoi(minor) != 9))
			_version = "HTTP/1.0";

		return (0);
	}

	int
	Request::validateURI()
	{
		if (_URI.empty())
			return (-1);
		if (_URI[0] != '/' || _URI[0] == ' ')
			return (-1);
		if (_URI.size() > 2048)
			return (-1);
		for (size_t i = 0; i < _URI.size(); i++)
		{
			if (_URI[i] <= 31 || _URI[i] == 127)
				return (-1);
			if (_URI[i] == '%')
			{
				if (i + 2 >= _URI.size())
					return (-1);
				if (!isHexDigits(_URI[i + 1]) || !isHexDigits(_URI[i + 2]))
					return (-1);
				i += 2;
			}
				return (-1);
			if (!isAllowedURICharacter(_URI[i]))
				return (-1);
		}

		return (0);
	}

	int
	Request::validateHeaders()
	{
		std::map<std::string, std::string>::iterator it;

		for (it = _generalHeaders.begin(); it != _generalHeaders.end(); ++it)
			if (!it->second.empty() && it->second[0] == ' ')
				return (-1);
		for (it = _requestHeaders.begin(); it != _requestHeaders.end(); ++it)
			if (!it->second.empty() && it->second[0] == ' ')
				return (-1);
		for (it = _entityHeaders.begin(); it != _entityHeaders.end(); ++it)
			if (!it->second.empty() && it->second[0] == ' ')
				return (-1);

		return (0);
	}

	// Utils //
	int
	Request::getlineCRLF(std::istream &stream, std::string &line)
	{
		if (!std::getline(stream, line))
			return (-1);

		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		return (0);
	}

	bool
	Request::isAllDigits(std::string const &str)
	{
		if (str.empty())
			return (false);

		for (int i = 0; i < str.size(); i++)
			if (!std::isdigit((unsigned char)str[i]))
				return (false);

		return (true);
	}

	bool
	Request::isHexDigits(char c) {
		return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
	}

	bool
	Request::isAllowedURICharacter(char c)
	{
		if (std::isalnum(c))
			return true;

		switch (c)
		{
			case '/': case '-': case '_': case '.': case '~':
			case '?': case '&': case '=': case '%':
				return true;
			default:
				return false;
		}
	}

	// Getters //
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

	std::string const &
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

	std::string const	&
	Request::getRequestHeaderValue(std::string const &key)
	const {
		HeaderMap::const_iterator it = _requestHeaders.find(key);
		if (it != _requestHeaders.end())
			return (it->second);
		else
			return (SP);
	}

	std::string const	&
	Request::getGeneralHeaderValue(std::string const &key)
	const {
		HeaderMap::const_iterator it = _generalHeaders.find(key);
		if (it != _generalHeaders.end())
			return (it->second);
		else
			return (SP);
	}

	std::string const	&
	Request::getEntityHeaderValue(std::string const &key)
	const {
		HeaderMap::const_iterator it = _entityHeaders.find(key);
		if (it != _entityHeaders.end())
			return (it->second);
		else
			return (SP);
	}

	Http::HeaderMap const &
	Request::getRequestHeaders()
	const {
		return (_requestHeaders);
	}
}
