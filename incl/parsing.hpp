#ifndef PARSING_HPP
#define PARSING_HPP

#include "Config.hpp"
#include "TokenStream.hpp"


Config::Server				parseServer(TokenStream &ts);
Config::Page				parseErrorPage(TokenStream &ts);
Config::Server::Location	parseLocation(TokenStream &ts);
std::string					parsePath(TokenStream &ts);
std::string					parseHost(std::string hostPort, size_t colonPos);
int							parsePort(std::string hostPort, size_t colonPos);
std::string					parseRoot(TokenStream &ts);
size_t						parseClientMaxBodySize(TokenStream &ts);
Config::Page				parseReturnURL(TokenStream &ts);
int							parseRedirectStatus(TokenStream &ts);
bool						parseAutoIndex(TokenStream &ts);

std::string					parseUploadDIR(TokenStream &ts);
std::string					parseIndex(TokenStream &ts);
std::string					parseCgiEXT(TokenStream &ts);
std::string					parseCgiPath(TokenStream &ts);
std::string					parseAllowedMethod(TokenStream &ts);
std::string					parseIndexFile(TokenStream &ts);

#endif
