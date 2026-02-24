#include "HttpRequest.hpp"

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
	Request::parse(std::string request)
	{
		std::istringstream stream(request);
		std::string line;

		getlineCRLF(stream, line);

		parseRequestLine(line);

		while (getlineCRLF(stream, line))
		{
			size_t colon = line.find(':');
			if (colon == std::string::npos)
				throw std::runtime_error("Colon not found"); // MOET HIER RUNTIME ERROR????
			std::string	key = line.substr(0, colon);
			std::string value = line.substr(colon + 1);
			try {
				HeaderHandlers[key](value);
			} catch (std::out_of_range const &e) {
			}
			
		}

		parseEntityBody(stream);

		return (0);
	}

	bool
	Request::parseRequestLine(std::string const &line)
	{
		size_t sp1 = line.find(' ');

		if (sp1 == std::string::npos)
			return (false);

		_method = line.substr(0, sp1);

		size_t sp2 = line.find(' ', sp1 + 1);

		if (sp2 == std::string::npos)
		{
			if (_method != "GET")
				return (false);
			_URI = line.substr(sp1 + 1);
			_version = "HTTP/0.9";
		}
		else
		{
			if (line.find(' ', sp2 + 1))
				return (false);
			_URI = line.substr(sp1 + 1, sp2 - sp1 - 1);
			if (!validHTTPVersion(line.substr(sp2 + 1)))
				return (false);
			_version = "HTTP/1.0";
		}
		return (true);
	}

	// Parse headers
	// General headers: Date, Pragma
	// Request headers: Authorization, From, If-Modified-Since, Referer, User-Agent
	// Entity headers: Allow, Content-Encoding, Content-Length, Content-Type, Expires, Last-Modified
	// void
	// Request::parseHeaders(std::string const &line)
	// {
	// 	size_t colon = line.find(':');
	// 	std::string	key = line.substr(0, colon);
	// 	std::string value = line.substr(colon + 1);
	// 	// trim leading whitespaces
	// 	_requestHeaders.insert(std::make_pair(key, value));
	// }

	bool
	Request::parseEntityBody(std::istream &stream)
	{
		std::map<std::string, std::string>::iterator it = _requestHeaders.find("Content-Length");
		if (it == _requestHeaders.end())
			return ;

		size_t	contentLength = std::stoul(it->second);
		_entityBody.resize(contentLength);
		stream.read(_entityBody.data(), contentLength); //data returns pointer to where entityBody is stored
		if (stream.gcount() != contentLength)
			return (false);
		return (true);
	}

	// Parsing utils //
	bool
	Request::validHTTPVersion(const std::string &version)
	{
		if (version.find("HTTP/") == std::string::npos)
			return (false);

		size_t	dot = version.find(".");
		if (dot == 5 || version.size() <= dot + 5) // returns false if: HTTP/.9 or HTTP/1.
			return (false);

		for (int i = 5; i < dot; i++)
			if (!std::isdigit((unsigned char)version[i]))
				return (false);

		for (int i = dot + 1; i < version.size(); i++)
			if (!std::isdigit((unsigned char)version[i]))
				return (false);
	}

	bool
	Request::getlineCRLF(std::istream &stream, std::string &line)
	{
		if (!std::getline(stream, line))
			return (false);

		if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);

		return (true);
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

	std::string
	Request::getHost(std::string const &key) // DEZE INFO VOOR CGI KOMT UIT CONFIG NIET REQUEST!!
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
