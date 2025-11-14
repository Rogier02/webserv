#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// C++
# include <unordered_map>
# include <string>

class HttpRequest
{

	public:
		enum	RequestType: uint8_t {
			GET,
			POST,
			DELETE,
		};

	private:
		static const std::unordered_map<std::string, RequestType>	Methods = {
			{"GET", RequestType::GET},
			{"POST", RequestType::POST},
			{"DELETE", RequestType::DELETE},
		};

	public:
		void	execGet();
		void	execPost();
		void	execDelete();

	public:
		//method
	
	private:
		RequestType	_type;

};

#endif
