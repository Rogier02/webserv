#ifndef HTTP_v1_0_HPP
# define HTTP_v1_0_HPP

# include <string>
# include <vector>
# include <map>

namespace Http {
	std::map<float, std::string>	Versions	= {
		{0.9f, "0.9"},
		{1.0f, "1.0"},
	};

	std::map<u_int8_t, std::string>	StatusCodes	= {
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
}

#endif