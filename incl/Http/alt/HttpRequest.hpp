#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// C++
# include <string>
# include <map>
// webserv
# include "Http_v1_0.hpp"

class	HttpRequest
{
	public:
		HttpRequest() = default;
		HttpRequest(HttpRequest const &) = delete;
		HttpRequest(HttpRequest &&) = delete;
		HttpRequest(std::string const &request);
		~HttpRequest() = default;

	public:
		std::string	toString() const;

	private:
		std::string	_method;
			// "GET", "HEAD", "POST"
		std::string	_URI;
			// "/relative/path/to/resource"
		std::string	_version;
			// 0.9, 1.0

		std::map<std::string, std::string>	_generalHeaders;
			// date, pragma
		std::map<std::string, std::string>	_requestHeaders;
			// authorisation, from, if-modified-since, referer, user-agent
		std::map<std::string, std::string>	_entityHeaders;
			// allow, content-endcoding, content-length, content-type, expires, last-modified

		std::string	_entityBody;

	public:
	// GETTERS
		std::string getMethod() const;
		std::string getQueryString() const;
		std::string getContentType() const;
		std::string getUri() const;
		std::string getScriptName() const;
		std::string getContentLength() const;
		std::string	getPort() const;
		std::string getHeader(std::string header) const;
		// set/get
};

#endif
