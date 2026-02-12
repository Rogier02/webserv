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

	Request::Request(
		std::string const &request
	) {
		(void)request;
		// parse request line for method, URI, version, otherwise v0.9
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

}
