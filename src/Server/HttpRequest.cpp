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

		// Parse request line for method, URI, version, otherwise v0.9
		size_t i = line.find(' ');
		size_t k = line.find(' ', i + 1);

		_method = line.substr(0, i);
		_URI = line.substr(i + 1, k - i - 1);
		_version = line.substr(k + 1);// 0.9?

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
			char buffer[contentLength];
			stream.read(buffer, contentLength);
			_entityBody.assign(buffer, stream.gcount());
		}

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
