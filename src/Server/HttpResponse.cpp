#include "HttpResponse.hpp"

namespace Http {
	Response::Response()
		:	Message("1.0")
		,	_statusCode(200)
		,	_reasonPhrase(Statuses[_statusCode])
	{
		_entityHeaders["content-type"] = "text/html";
		_entityHeaders["content-length"] = "0";
	}

	std::string
	Response::toString()
	const {
		if (_version == "0.9")
			return (_entityBody);

		std::string	statusLine;
		std::string	headers;

		statusLine = "HTTP/" + _version + SP + std::to_string(_statusCode) + SP + _reasonPhrase + CRLF;

		writeHeaders(headers, _generalHeaders);
		writeHeaders(headers, _responseHeaders);
		writeHeaders(headers, _entityHeaders);

		return (statusLine + headers + CRLF + _entityBody);
	}

	int
	Response::err(
		u_int16_t statusCode)
	{
		setStatus(statusCode);

		_entityBody	=
			"<html>\n"
			"<head><title>" + std::to_string(statusCode) + " " + _reasonPhrase + "</title></head>\n"
			"<body>\n"
			"<center><h1>" + std::to_string(statusCode) + " " + _reasonPhrase + "</h1></center>\n"
			"<hr><center>webserv/1.0</center>\n"
			"</body>\n"
			"</html>\n";

		return (0);
	}

	int
	Response::setVersion(
		std::string const &version)
	{
		_version = version;
		return (0);
	}

	int
	Response::setStatus(
		u_int16_t statusCode)
	{
		_statusCode		= statusCode;
		_reasonPhrase	= Statuses[statusCode];
		return (0);
	}

	int
	Response::setEntityBody(
		std::string const &content)
	{
		_entityBody = content;

		_entityHeaders["content-length"] = std::to_string(content.length());
		// other entity headers

		return (0);
	}

	int
	Response::setResponseHeaderValue(std::string const &key, std::string const &value) {
		_responseHeaders[key] = value;
		return (0);
	}
}
