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

		std::getline(stream, line);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		parseRequestLine(line);

		while (std::getline(stream, line))
		{
			if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
			if (line.empty())
				break;

			size_t colon = line.find(':');
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

	void
	Request::parseRequestLine(std::string const &line)
	{
		size_t i = line.find(' ');
		size_t k = line.find(' ', i + 1);

		_method = line.substr(0, i);
		if (k == std::string::npos) {
			_URI = line.substr(i + 1);
			_version = "HTTP/0.9";
		}
		else {
			_URI = line.substr(i + 1, k - i - 1);
			if (validHTTPVersion(line.substr(k + 1)))
				_version = "HTTP/1.0";
			else
				return ; // Throw error something maybe?? stop parsing?
		}
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

	void
	Request::parseEntityBody(std::istream &stream)
	{
		std::map<std::string, std::string>::iterator it = _requestHeaders.find("Content-Length");
		if (it == _requestHeaders.end())
			return ;

		size_t	contentLength = std::stoul(it->second);
		_entityBody.resize(contentLength);
		stream.read(_entityBody.data(), contentLength); //data returns pointer to where entityBody is stored
		if (stream.gcount() != contentLength)
			throw std::runtime_error("Incomplete body!"); // CHECK IF THIS THROW HAPPENS CORRECT
	}

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
