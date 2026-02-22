#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// CPP
# include <string>
// webserv
# include "HttpMessage.hpp"

namespace Http {
	class	Response : public Message {
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
				// "OK", "Not Found" etc.

			HeaderMap	_responseHeaders;
				// location, server, www-authenticate

		public:
			void	setVersion(std::string const &version);
			void	setStatus(u_int16_t	statusCode);
			void	setEntityBody(std::string const &content);

			void	setResponseHeaderValue(std::string const &key, std::string const &value);

			void	setGeneralHeaderValue(std::string const &key, std::string const &value);
			void	setEntityHeaderValue(std::string const &key, std::string const &value);
	};
}

#endif