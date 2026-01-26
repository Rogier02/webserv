#include "ErrorPages.hpp"

std::string
ErrorPages::getBody(int code)
{
	const std::string title		= _pages[code];
	const std::string codeStr	= std::to_string(code);

	std::string htmlBody =
		"<html>\n"
		"<head><title>" + codeStr + " " + title + "</title></head>\n"
		"<body>\n"
		"<center><h1>" + codeStr + " " + title + "</h1></center>\n"
		"<hr><center>webserv/1.0</center>\n"
		"</body>\n"
		"</html>\n";

	return (htmlBody);
}

std::string ErrorPages::getTitle(int code) {
	return (_pages[code]);
}
