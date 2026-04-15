#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// CPP
# include <string>
# include <map>
// webserv
# include "HttpMessage.hpp"

namespace Http {
	class	Response : public Message {
		private:
			static const std::map<std::string, std::string> FileTypes;

		public:
			Response();
			Response(Response const &) = delete;
			Response(Response &&) = delete;
			~Response() = default;

		public:
			std::string toString() const override;

			void	err(u_int16_t statusCode);

		private:
			u_int16_t	_statusCode;
				// 2xx, 3xx, 4xx, 5xx
			std::string	_reasonPhrase;
				// "OK", "Not Found", etc.

			HeaderMap	_responseHeaders;
				// location, server, www-authenticate

		public:

			void	setVersion(std::string const &version);
			void	setStatus(u_int16_t	statusCode);
			void	setEntityBody(std::string const &content, std::string const &file = "");

			void	setResponseHeaderValue(std::string const &key, std::string const &value);

		private:
			static std::string const	&_getContentType(std::string const &filename);

	// TODO: should these do something?
			// void	setGeneralHeaderValue(std::string const &key, std::string const &value);
			// void	setEntityHeaderValue(std::string const &key, std::string const &value);
	};
}

#endif