#include "ErrorPageHandler.hpp"
#include <fstream>
#include <sstream>

const std::string ErrorPageHandler:: _errorPageDirectory = "./webpages/";

ErrorPageHandler::ErrorPageHandler()
{}

ErrorPageHandler::~ErrorPageHandler()
{}

std::string
ErrorPageHandler::getErrorPage(int statusCode)
{
	auto it = _errorPages.find(statusCode);
	if (it != _errorPages.end())
		return (it->second);
	return (getDefaultErrorPage(statusCode));
}

void
ErrorPageHandler::setErrorPage(int statusCode, const std::string &filePath)
{
	std::string content = loadErrorPageFromFile(filePath);
	if (!content.empty())
		_errorPages[statusCode] = content;
}