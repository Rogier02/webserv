#include "TokenStream.hpp"
#include "Config.hpp"
#include "log.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Config::Server::Location	parseLocation(TokenStream &ts){
	Config::Server::Location location;

	location.path = parsePath(ts);
	while (!ts.atEnd() && ts.current().text != "}"){
		if (ts.current().text == "{")
			ts.next();
		if (ts.current().text == "root")
			location.root = parseRoot(ts);
		if (ts.current().text == "client_max_body_size")
			location.clientMaxBodySize = parseClientMaxBodySize(ts);
		if (ts.current().text == "return")
			location.returnURL = parseReturnURL(ts);
		if (ts.current().text == "redirectStatus")
			location.redirectStatus = parseRedirectStatus(ts);
		if (ts.current().text == "autoindex")
			location.autoindex = parseAutoIndex(ts);
		if (ts.current().text == "upload_dir")
			location.uploadDir = parseUploadDIR(ts);
		if (ts.current().text == "index")
			location.index = parseIndex(ts);
		if (ts.current().text == "cgi_ext")
			location.cgiEXT = parseCgiEXT(ts);
		if (ts.current().text == "cgi_path")
			location.cgiPath = parseCgiPath(ts);
		if (ts.current().text == "allowed_methods")
			location.allowedMethods.push_back(parseAllowedMethods(ts));
		if (ts.current().text == "index_files")
			location.indexFiles.push_back(parseIndexFiles(ts));
	}
	return(location);
}

std::string	parsePath(TokenStream &ts){
	ts.next();
	std::string path = ts.current().text;
	ts.next();
	return (path);
}

std::string	parseRoot(TokenStream &ts){
	ts.next();
	std::string root = ts.current().text;
	ts.next();
	return (root);
}

size_t	parseClientMaxBodySize(TokenStream &ts){
	ts.next();
	size_t size = 0;
	char unit = ts.current().text.back();
	std::string number = ts.current().text;
		
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
	ts.next();
	return (size);
}

std::string	parseReturnURL(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}

int	parseRedirectStatus(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}


bool	parseAutoIndex(TokenStream &ts){
	ts.next();
	bool autoindex = false;
	if (ts.current().text == "off")
		autoindex = false;
	else if (ts.current().text == "on")
		autoindex = true;
	else {
		LOG("[Config Error] Line " << ts.current().lineNbr << ": \"" << ts.current().text << "\" -> Unknown option");
	}
	ts.next();
	return (autoindex);
}


std::string	parseUploadDIR(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}


std::string	parseIndex(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}


std::string	parseCgiEXT(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}

std::string	parseCgiPath(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}

std::string	parseAllowedMethods(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}


std::string	parseIndexFiles(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}
