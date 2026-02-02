#include "Http_v1_0.hpp"

namespace Http {
	int
	Message::setEntityBody(
		std::string const &content
	) {
		_entityBody = content;

		_entityHeaders["content-length"] = std::to_string(content.length());
		// other entity headers

		return (0);
	}

	std::string const &
	Message::getVersion()
	const {
		return (_version);
	}

	void
	Message::writeHeaders(
		std::string &dest,
		HeaderMap const &headerMap
	) {
		for (std::pair<std::string, std::string> const &header : headerMap)
			dest += header.first + ":" + header.second + CRLF;
	}

	Request::Request(
		std::string const &request
	) {
		// parse request line for method, URI, version, otherwise v0.9
	}

	Response::Response()
		:	_statusCode(200)
		,	_reasonPhrase(StatusCodes[_statusCode])
	{}

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

		statusLine = _version + SP + std::to_string(_statusCode) + SP + _reasonPhrase + CRLF;

		writeHeaders(headers, _generalHeaders);
		writeHeaders(headers, _responseHeaders);
		writeHeaders(headers, _entityHeaders);

		return (statusLine + headers + CRLF + _entityBody);
	}
}
