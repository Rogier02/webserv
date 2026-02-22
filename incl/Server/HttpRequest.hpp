#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// CPP
# include <string>
# include <sstream>
# include <functional>

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

		private:
			std::string	_method;
			// "GET", "HEAD", "POST"
			std::string	_URI;
			// "/relative/path/to/resource"
			
			HeaderMap	_requestHeaders;
			// authorisation, from, if-modified-since, referer, user-agent

		public:
			int		parse(std::string request);
			void	parseRequestLine(std::string const &line);
			void	parseEntityBody(std::istream &stream);
			bool	validHTTPVersion(std::string const &version);

		using	HeaderHandler = std::function<void (std::string const &)>;
		std::map<std::string, HeaderHandler>
		HeaderHandlers = {
			{"Date",
				[this](std::string const &value)
				{ _generalHeaders["Date"] = value; }},
			{"Pragma",
				[this](std::string const &value)
				{ _generalHeaders["Pragma"] = value; }},
			{"Authorization",
				[this](std::string const &value)
				{ _requestHeaders["Authorization"] = value; }},
			{"From",
				[this](std::string const &value)
				{ _requestHeaders["From"] = value; }},
			{"If-Modified-Since",
				[this](std::string const &value)
				{ _requestHeaders["If-Modified-Since"] = value; }},
			{"Referer",
				[this](std::string const &value)
				{ _requestHeaders["Referer"] = value; }},
			{"User-Agent",
				[this](std::string const &value)
				{ _requestHeaders["User-Agent"] = value; }},
			{"Allow",
				[this](std::string const &value)
				{ _entityHeaders["Allow"] = value; }},
			{"Content-Encoding",
				[this](std::string const &value)
				{ _entityHeaders["Content-Encoding"] = value; }},
			{"Content-Length",
				[this](std::string const &value)
				{ _entityHeaders["Content-Length"] = value; }},
			{"Content-Type",
				[this](std::string const &value)
				{ _entityHeaders["Content-Type"] = value; }},
			{"Expires",
				[this](std::string const &value)
				{ _entityHeaders["Expires"] = value; }},
			{"Last-Modified",
				[this](std::string const &value)
				{ _entityHeaders["Last-Modified"] = value; }},
			};

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