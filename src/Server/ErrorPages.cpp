#include "ErrorPages.hpp"

const std::string
ErrorPages::_directory = "./defaultPages/Error";

std::map<int, ErrorPages::Page>
ErrorPages::_pages = {
	// {000, {000, "Unkown Error"			, ""			, ""}},
	// {400, {400, "Bad Request"			, ""			, ""}},
	{404, {404, "Not Found"				, "404.html"	, ""}},
	// {500, {500, "Internal Server Error"	, ""			, ""}},
	// {502, {502, "Bad Gateway"			, ""			, ""}},
	// {503, {503, "Service Unavailable"	, ""			, ""}},
};

ErrorPages::ErrorPages()
{
	for (std::pair<int, ErrorPages::Page> pair : _pages)
	{
		Page	&page = pair.second;

		if (page.code != pair.first
		||	page.path.empty())
			throw std::logic_error("invalid error page registration");

		std::ifstream	file(page.path);
		if (file.is_open())
			throw std::runtime_error("failed to open error page" + page.path);

		std::stringstream buffer;
		buffer << file.rdbuf();
		page.content = buffer.str();
	}
}

ErrorPages::~ErrorPages()
{}

ErrorPages::Page const &
ErrorPages::getPage(int code)
{
	std::map<int, Page>::const_iterator	iterator = _pages.find(code);
	if (iterator == _pages.end())
		return (_pages.find(000)->second);
	return (iterator->second);
}

std::string
ErrorPages::getErrorPage(int code)
{
	const std::string title = getPage(code).title;

	std::string html =
		"<html>\n"
		"<head><title>" + std::to_string(code) + " " + title + "</title></head>\n"
		"<body>\n"
		"<center><h1>" + std::to_string(code) + " " + title + "</h1></center>\n"
		"<hr><center>webserv/1.0</center>\n"
		"</body>\n"
		"</html>\n";

	return (html);
}
