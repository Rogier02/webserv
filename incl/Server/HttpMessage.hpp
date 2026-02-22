#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

// CPP
# include <string>
# include <map>

namespace Http {
	constexpr	std::string	SP = " ";
	constexpr	std::string CRLF = "\r\n";

	using	HeaderMap = std::map<std::string, std::string>;

	static std::map<u_int16_t, std::string>	Statuses	= {
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
	};

	class	Message {
		public:
			Message() = default;
			Message(std::string const &version);
			Message(Message const &) = delete;
			Message(Message &&) = delete;
			virtual ~Message() = default;

		public:
			virtual std::string	toString() const = 0;

			HeaderMap const		&getGeneralHeaders() const;
			HeaderMap const		&getEntityHeaders() const;

		protected:
			std::string	_version;

			HeaderMap	_generalHeaders;
				// date, pragma
			HeaderMap	_entityHeaders;
				// allow, content-endcoding, content-length, content-type, expires, last-modified

			std::string	_entityBody;

		protected:
			static void	writeHeaders(std::string &dest, HeaderMap const &headerMap);
	};
}

#endif