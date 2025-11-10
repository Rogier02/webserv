#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <unordered_map>
# include <string>

enum	RequestType: uint8_t {
	GET,
	POST,
	DELETE,
};

static const std::unordered_map<std::string, RequestType>	Method = {
	{"GET", RequestType::GET},
	{"POST", RequestType::POST},
	{"DELETE", RequestType::DELETE},
};

#endif
