#include "HttpResponse.hpp"

namespace Http {

	const std::string	Response::DefaultErrorPage = "www/errorPages/defaultErrorPage.html";

	const std::map<std::string, std::string> Response::FileTypes = {
		{".css", "text/css"},
		{".html", "text/html"},
		{".ico", "image/icon"},
		{".jpeg", "image/jpeg"},
		{".jpg", "image/jpeg"},
		{".png", "image/png"},
	};

	Response::Response()
		:	Message("1.0")
		,	_statusCode(200)
		,	_reasonPhrase(Statuses.at(_statusCode))
	{
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

	void
	Response::err(
		u_int16_t statusCode)
	{
		setStatus(statusCode);

		std::string entityBody = IO::getFileContent(DefaultErrorPage);

		std::string placeholder1 = "statusCode";
		size_t pos = entityBody.find(placeholder1);

		if (pos != std::string::npos) {
			entityBody.replace(pos, placeholder1.size(), std::to_string(statusCode));
		}

		std::string placeholder2 = "reasonPhrase";
		pos = entityBody.find(placeholder2);

		if (pos != std::string::npos) {
			entityBody.replace(pos, placeholder2.size(), _reasonPhrase);
		}

		setEntityBody(entityBody);
	}

	std::string const &
	Response::_getContentType(std::string const &filename)
	{
		::size_t	dot = filename.find_last_of('.');
		if (dot == std::string::npos)
			return (FileTypes.at(".html"));

		std::string	extension = filename.substr(dot);
		if (!FileTypes.contains(extension))
			return (FileTypes.at(".html"));

		return (FileTypes.at(extension));
	}

	void Response::setVersion(std::string const &version) {
		_version = version;
	}

	void
	Response::setStatus(
		u_int16_t statusCode)
	{
		_statusCode		= statusCode;
		_reasonPhrase	= Statuses.at(_statusCode);
	}

	void
	Response::setEntityBody(
		std::string const &content,
		std::string const &file)
	{
		_entityBody = content;

		_entityHeaders["content-length"]	= std::to_string(content.length());
		_entityHeaders["content-type"]		= _getContentType(file);
	}

	void Response::setResponseHeaderValue(std::string const &key, std::string const &value)	{
		_responseHeaders[key] = value;
	}
}
