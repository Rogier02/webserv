#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// C++
#include <string>
#include <map>

class	HttpResponse
{
	public:
		HttpResponse(int statusCode = 200);
		~HttpResponse() = default;

	private:
		int									_statusCode;
		std::string							_reasonPhrase;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

	public:
		void	setStatus(int code);
		void	setHeader(const std::string& key, const std::string& value);
		void	setBody(const std::string& body);
		void	setContentType(const std::string& type);

		std::string toString() const;
};

#endif