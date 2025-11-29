#include "ErrorPageHandler.hpp"
#include <fstream>
#include <sstream>

const std::string ErrorPageHandler::_errorPageDirectory = "./webpages/";

ErrorPageHandler::ErrorPageHandler()
{}

ErrorPageHandler::~ErrorPageHandler()
{}

std::string
ErrorPageHandler::getErrorPage(int statusCode)
const
{
    auto it = _errorPages.find(statusCode);
    if (it != _errorPages.end())
        return (it->second);
    return (getDefaultErrorPage(statusCode));
}

void
ErrorPageHandler::setErrorPage(int statusCode, const std::string& filePath)
{
    std::string content = loadErrorPageFromFile(filePath);
    if (!content.empty())
        _errorPages[statusCode] = content;
}

std::string
ErrorPageHandler::loadErrorPageFromFile(const std::string& filePath) const
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return ("");
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return (buffer.str());
}

std::string
ErrorPageHandler::getDefaultErrorPage(int statusCode) const
{
    std::string reasonPhrase;

    switch (statusCode) {
        case 400:
            reasonPhrase = "Bad Request";
            break;
        case 404:
            reasonPhrase = "Not Found";
            break;
        case 500:
            reasonPhrase = "Internal Server Error";
            break;
        case 502:
            reasonPhrase = "Bad Gateway";
            break;
        case 503:
            reasonPhrase = "Service Unavailable";
            break;
        default:
            reasonPhrase = "Unknown Error";
            break;
    }

    std::string html = 
        "<html>\n"
        "<head><title>" + std::to_string(statusCode) + " " + reasonPhrase + "</title></head>\n"
        "<body>\n"
        "<center><h1>" + std::to_string(statusCode) + " " + reasonPhrase + "</h1></center>\n"
        "<hr><center>webserv/1.0</center>\n"
        "</body>\n"
        "</html>\n";

    return (html);
}