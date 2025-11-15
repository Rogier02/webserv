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
		if (ts.current().text == "root"){
			location.root = parseRoot(ts);
			continue;
		}
		if (ts.current().text == "client_max_body_size"){
			location.clientMaxBodySize = parseClientMaxBodySize(ts);
			continue;
		}
		if (ts.current().text == "return"){
			location.returnURL = parseReturnURL(ts);
			continue;
		}
		if (ts.current().text == "redirectStatus"){
			location.redirectStatus = parseRedirectStatus(ts);
			continue;
		}
		if (ts.current().text == "autoindex"){
			location.autoindex = parseAutoIndex(ts);
			continue;
		}
		if (ts.current().text == "upload_dir"){
			location.uploadDir = parseUploadDIR(ts);
			continue;
		}
		if (ts.current().text == "index"){
			location.index = parseIndex(ts);
			continue;
		}
		if (ts.current().text == "cgi_ext"){
			location.cgiEXT = parseCgiEXT(ts);
			continue;
		}
		if (ts.current().text == "cgi_path"){
			location.cgiPath = parseCgiPath(ts);
			continue;
		}
		if (ts.current().text == "allowed_methods"){
			location.allowedMethods.push_back(parseAllowedMethods(ts));
			continue;
		}
		if (ts.current().text == "index_files"){
			location.indexFiles.push_back(parseIndexFiles(ts));
			continue;
		}
	}
	return(location);
}

std::string	parsePath(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}

std::string	parseRoot(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
}

size_t	parseClientMaxBodySize(TokenStream &ts){
	ts.next();

	ts.next();
	return ();
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

	ts.next();
	return ();
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
