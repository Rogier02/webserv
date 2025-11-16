#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

// c++ headers
#include <string>
#include <map>

# define TYPE
# define CONTENT_LENGTH
# define CONNECTION
# define DATE
# define SERVER

//HttpResponse Class
class HttpResponse {
	
	//attributes
	private: 
		int _statusCode;
		std::string _reasonPhrase;
		std::map<std::string, std::string> _headers;
		std::string _body;

	// Methods
	public:
		
		HttpResponse(int statusCode = 200);

		void	setStatus(int code);
		void	setHeader(const std::string& key, const std::string& value);
		void	setBody(const std::string& body);
		void	setContentType(const std::string& type);

		std::string toString() const;
};

#endif