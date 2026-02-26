#include "HttpMessage.hpp"

namespace Http {
	Message::Message(
		std::string const &version)
		:	_version(version)
	{
		// _entityHeaders["content-length"] = "0";
	}

	void
	Message::writeHeaders(
		std::string		&dest,
		HeaderMap const	&headerMap)
	{
		for (std::pair<const std::string, std::string> const &header : headerMap)
			dest += header.first + ":" + header.second + CRLF;
	}

	Http::HeaderMap const &
	Message::getGeneralHeaders()
	const {
		return (_generalHeaders);
	}

	Http::HeaderMap const &
	Message::getEntityHeaders()
	const {
		return (_entityHeaders);
	}
}
