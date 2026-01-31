#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// C++
# include <string>
# include <map>
// webserv
# include <Http_v1_0.hpp>

class	HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(HttpResponse const &) = delete;
		HttpResponse(HttpResponse &&) = delete;
		~HttpResponse() = default;

	public:
		std::string toString() const;

	private:
		std::string	_version;
			// 0.9, 1.0
		std::string	_statusCode;
			// 2xx, 3xx, 4xx, 5xx
		std::string	_reasonPhrase;
			// "OK", "Not Found" etc.

		std::map<std::string, std::string>	_generalHeaders;
			// date, pragma
		std::map<std::string, std::string>	_responseHeaders;
			// location, server, www-authenticate
		std::map<std::string, std::string>	_entityHeaders;
			// allow, content-endcoding, content-length, content-type, expires, last-modified

		std::string	_entityBody;

	public:
		// set/get
};

#endif