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
			int		parseRequestLine(std::string const &line);
			int		parseEntityBody(std::istream &stream);
			int		parseHeaders(std::istream &stream, std::string &line);
			int		getlineCRLF(std::istream &stream, std::string &line);
			int		validateParseRequest();
			int		validateHTTPVersion();
			int		validateURI();
			int		validateHeaders();
			bool	isAllDigits(std::string const &str);
			bool	isHexDigits(char c);
			bool	isAllowedURICharacter(char c);
			
		using	HeaderHandler = std::function<void (std::string const &)>;
		std::map<std::string, HeaderHandler>
		HeaderHandlers = {
			{"date",
				[this](std::string const &value)
				{ _generalHeaders["date"] = value; }},
			{"pragma",
				[this](std::string const &value)
				{ _generalHeaders["pragma"] = value; }},
			{"authorization",
				[this](std::string const &value)
				{ _requestHeaders["authorization"] = value; }},
			{"from",
				[this](std::string const &value)
				{ _requestHeaders["from"] = value; }},
			{"if-modified-since",
				[this](std::string const &value)
				{ _requestHeaders["if-modified-since"] = value; }},
			{"referer",
				[this](std::string const &value)
				{ _requestHeaders["referer"] = value; }},
			{"user-agent",
				[this](std::string const &value)
				{ _requestHeaders["user-agent"] = value; }},
			{"allow",
				[this](std::string const &value)
				{ _entityHeaders["allow"] = value; }},
			{"content-encoding",
				[this](std::string const &value)
				{ _entityHeaders["content-encoding"] = value; }},
			{"content-length",
				[this](std::string const &value)
				{ _entityHeaders["content-length"] = value; }},
			{"content-type",
				[this](std::string const &value)
				{ _entityHeaders["content-type"] = value; }},
			{"expires",
				[this](std::string const &value)
				{ _entityHeaders["expires"] = value; }},
			{"last-modified",
				[this](std::string const &value)
				{ _entityHeaders["last-modified"] = value; }},
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