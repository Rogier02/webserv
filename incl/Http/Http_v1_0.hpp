#ifndef HTTP_v1_0_HPP
# define HTTP_v1_0_HPP

# include <string>
# include <map>

# include <functional>

namespace Http {
	constexpr	std::string	SP = " ";
	constexpr	std::string CRLF = "\r\n";

	using	HeaderMap = std::map<std::string, std::string>;

	static std::map<u_int8_t, std::string>	StatusCodes	= {
		{200, "OK"},
		{201, "Created"},
		{202, "Accepted"},
		{204, "No Content"},

		{301, "Moved Permanently"},
		{302, "Moved Temporarily"},
		{304, "Not Modified"},

		{400, "Bad Request"},
		{401, "Unauthorised"},
		{403, "Forbidden"},
		{404, "Not Found"},

		{500, "Internal Server Error"},
		{501, "Not Implemented"},
		{502, "Bad Gateway"},
		{503, "Service Unavailable"},
	};// maybe move this into Response class?

	class	Message {
		protected:
			// using	HeaderMap = std::map<std::string, std::string>;

		public:
			Message() = default;
			Message(std::string const &version);
			Message(Message const &) = delete;
			Message(Message &&) = delete;
			virtual ~Message() = default;

		public:
			virtual std::string	toString() const = 0;

		protected:
			std::string	_version;

			HeaderMap	_generalHeaders;
				// date, pragma
			HeaderMap	_entityHeaders;
				// allow, content-endcoding, content-length, content-type, expires, last-modified

			std::string	_entityBody;

		protected:
			static void	writeHeaders(std::string &dest, HeaderMap const &headerMap);

			std::string const	&getGeneralHeaderValue(std::string const &key) const;
			std::string const	&getEntityHeaderValue(std::string const &key) const;
	};

	class	Request : public Message {
		public:
			Request() = delete;
			Request(Request const &) = delete;
			Request(Request &&) = delete;
			Request(std::string const &request);
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
			std::string const	&getVersion() const;
			std::string const	&getMethod() const;
			std::string const	&getEntityBody() const;
			std::string getURI() const;
			std::string getQueryString() const;
			std::string getScriptName() const;
			std::string getHost(std::string const &key) const;

			Http::HeaderMap const	&getRequestHeaders() const;
			std::string const		&getRequestHeaderValue(std::string const &key) const;
	};

	class	Response : public Message {
		public:
			Response(std::string const &version = "0.9", u_int8_t statusCode = 200);
			Response(Response const &) = delete;
			Response(Response &&) = delete;
			~Response() = default;

		public:
			std::string toString() const override;

		private:
			u_int8_t	_statusCode;
				// 2xx, 3xx, 4xx, 5xx
			std::string	_reasonPhrase;
				// "OK", "Not Found" etc.

			HeaderMap	_responseHeaders;
				// location, server, www-authenticate

		public:
			int	setVersion(std::string const &version);
			int	setStatus(std::string const &status);
			int	setEntityBody(std::string const &content);

			int	setGeneralHeaderValue(std::string const &key, std::string const &value);
			int	setEntityHeaderValue(std::string const &key, std::string const &value);

			//Rogier
			int setResponseHeaderValue(std::string const &key, std::string const &value);
			int setStatuscode(u_int8_t statusCode);
	};
}

#endif