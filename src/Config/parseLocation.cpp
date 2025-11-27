#include "parseLocation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Config::Server::Location
location(TokenStream &ts)
{
	Config::Server::Location location;

	location.path = parsePath(ts);
	ts.expect("{");
	while (!ts.atEnd() && ts.current().text != "}")
	{
		std::string current = ts.takeToken();
		if (current == "root")
			location.root = parseRoot(ts);
		else if (current == "client_max_body_size")
			location.clientMaxBodySize = parseClientMaxBodySize(ts);
		else if (current == "return")
			location.returnURL = parseReturnURL(ts);
		else if (current == "redirectStatus")
			location.redirectStatus = parseRedirectStatus(ts);
		else if (current == "autoindex")
			location.autoindex = parseAutoIndex(ts);
		else if (current == "upload_dir")
			location.uploadDir = parseUploadDIR(ts);
		else if (current == "index")
			location.index = parseIndex(ts);
		else if (current == "cgi_ext")
			location.cgiEXT = parseCgiEXT(ts);
		else if (current == "cgi_path")
			location.cgiPath = parseCgiPath(ts);
		else if (current == "allowed_methods")
		{
			while (!ts.isLastTokenOnLine())
				location.allowedMethods.push_back(parseAllowedMethod(ts));
			ts.checkSemicolon();
		}
		else if (current == "index_files")
		{
			while (!ts.isLastTokenOnLine())
				location.indexFiles.push_back(parseIndexFile(ts));
			ts.checkSemicolon();
		}
		else{
			LOG("[Config Error] \t\"" << ts.current().text << "\" -> Unknown directive on line " << ts.current().lineNbr << ": " << ts.getLine());
			ts.setIndex(ts.lastTokenOnLine() + 1);
		}
	}
	ts.expect("}");
	return(location);
}

std::string
parsePath(TokenStream &ts)
{
	std::string path = ts.takeToken();

	return (path);
}

std::string
parseRoot(TokenStream &ts)
{
	std::string root = ts.takeToken();
	ts.checkSemicolon();

	return (root);
}

size_t
parseClientMaxBodySize(TokenStream &ts)
{
	size_t size = 0;
	char unit = ts.current().text.back();
	std::string number = ts.takeToken();

	if (unit == 'k' || unit == 'm' || unit == 'g') {
		number = number.substr(0, number.size() - 1);
	} else {
		unit = 0;
	}

	size = std::stoul(number);

	switch(unit) {
		case 'k': size *= 1024; break;
		case 'm': size *= 1024 * 1024; break;
		case 'g': size *= 1024 * 1024 * 1024; break;
	}
	ts.checkSemicolon();
	return (size);
}

Config::Server::Page
parseReturnURL(TokenStream &ts)
{
	Config::Server::Page returnURL;
	returnURL.code = std::stoi(ts.takeToken());
	returnURL.path = ts.takeToken();
	ts.checkSemicolon();
	return (returnURL);
}

int
parseRedirectStatus(TokenStream &ts)
{
	int redirectStatus = std::stoi(ts.takeToken());
	ts.checkSemicolon();
	return (redirectStatus);
}

bool
parseAutoIndex(TokenStream &ts)
{
	std::string current = ts.takeToken();
	bool autoindex = false;

	if (current == "off")
	autoindex = false;
	else if (current == "on")
	autoindex = true;
	else {
		LOG("[Config Error] Line " << ts.current().lineNbr << ": \"" << current << "\" -> Unknown option");
	}
	ts.checkSemicolon();
	return (autoindex);
}

std::string
parseUploadDIR(TokenStream &ts)
{
	std::string uploadDIR = ts.takeToken();
	ts.checkSemicolon();
	return (uploadDIR);
}

std::string
parseIndex(TokenStream &ts)
{
	std::string index = ts.takeToken();
	ts.checkSemicolon();
	return (index);
}

std::string
parseCgiEXT(TokenStream &ts)
{
	std::string cgiEXT = ts.takeToken();
	ts.checkSemicolon();
	return (cgiEXT);
}

std::string
parseCgiPath(TokenStream &ts)
{
	std::string cgiPath = ts.takeToken();
	ts.checkSemicolon();
	return (cgiPath);
}

std::string
parseAllowedMethod(TokenStream &ts)
{
	std::string allowedMethod = ts.takeToken();
	return (allowedMethod);
}

std::string
parseIndexFile(TokenStream &ts)
{
	std::string indexFile = ts.takeToken();
	return (indexFile);
}