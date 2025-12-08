#ifndef PARSEHTTPREQUEST_HPP
#define PARSEHTTPREQUEST_HPP

#include "TokenStream"

class	ParseHttpRequest
{
	private:
		std::vector<std::string>	_log;
		TokenStream					_ts;

	public:
		ParseHttpRequest() = delete;
		ParseHttpRequest(ParseHttpRequest const &other) = delete;
		ParseHttpRequest(ParseHttpRequest &&other) = delete;
		// ParseHttpRequest(std::string const &filePath);
		ParseHttpRequest(std::istream& stream, int streamType);
		~ParseHttpRequest() = default;

	public:
		HttpRequest	httpRequest();
	
	private:
		void		parseHttpRequest(int clientFd);
		std::string	parseMethod(std::string method);
		HttpVersion	parseVersion(std::string version);
}

#endif