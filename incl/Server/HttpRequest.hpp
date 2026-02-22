#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// CPP
# include <string>
# include <sstream>
// webserv
# include "HttpMessage.hpp"

namespace Http {
	class	Request : public Message {
		public:
			Request() = default;
			Request(Request const &) = delete;
			Request(Request &&) = delete;
			~Request() = default;

		public:
			std::string	toString() const override;

			int	parse(std::string request);

		private:
			std::string	_method;
				// "GET", "HEAD", "POST"
			std::string	_URI;
				// "/relative/path/to/resource"

			HeaderMap	_requestHeaders;
				// authorisation, from, if-modified-since, referer, user-agent

		public:
			std::string const	&getVersion() const;
			std::string const	&getMethod() const;
			std::string const	&getURI() const;
			std::string const	&getEntityBody() const;
			std::string getQueryString() const;
			std::string getScriptName() const;
			std::string getHost(std::string const &key) const;

			HeaderMap const		&getRequestHeaders() const;
			std::string const	&getRequestHeaderValue(std::string const &key) const;

			std::string const	&getGeneralHeaderValue(std::string const &key) const;
			std::string const	&getEntityHeaderValue(std::string const &key) const;
	};
}

#endif