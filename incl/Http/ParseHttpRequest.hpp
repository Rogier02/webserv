#ifndef PARSEHTTPREQUEST_HPP
# define PARSEHTTPREQUEST_HPP

// C
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
// C++
# include <string>
# include <vector>
# include <functional>
# include <map>
# include <sstream>
// webserv
# include "HttpRequest.hpp"
# include "TokenStream.hpp"
# include "Logger.hpp"
# include "Config.hpp"

class	ParseHttpRequest
{
	private:
		std::vector<std::string>	_log;
		TokenStream					_ts;
		Config						&_config;

	public:
		ParseHttpRequest() = delete;
		ParseHttpRequest(ParseHttpRequest const &other) = delete;
		ParseHttpRequest(ParseHttpRequest &&other) = delete;
		ParseHttpRequest(std::istream& stream, int streamType, Config &config);
		~ParseHttpRequest() = default;

	public:
		HttpRequest	httpRequest();
	
	private:
		std::string					parseMethod(std::string method);
		HttpRequest::HttpVersion	parseVersion(std::string version);
};

#endif