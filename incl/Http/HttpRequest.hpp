#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// C++
# include <unordered_map>
# include <string>

struct	HttpRequest
{
	struct Header {
		std::string	headerName;
		std::string	headerValue;
	};

	struct HttpVersion {
		int	major;
		int	minor;
	};
	
	int					method;
	std::string			target;
	std::string			path;
	std::string			query;
	HttpVersion			version;
	std::vector<Header>	headers;

	void	parseHttpRequest(std::string request);
	void	parseRequestLine(std::string line);
};


// class HttpRequest
// {

// 	public:
// 		enum	RequestType: uint8_t {
// 			GET,
// 			POST,
// 			DELETE,
// 		};

// 	private:
// 		static const std::unordered_map<std::string, RequestType>	Methods = {
// 			{"GET", RequestType::GET},
// 			{"POST", RequestType::POST},
// 			{"DELETE", RequestType::DELETE},
// 		};

// 	public:
// 		void	execGet();
// 		void	execPost();
// 		void	execDelete();

// 	public:
// 		//method
	
// 	private:
// 		RequestType	_type;

// };

#endif
