#include <string>
#include <iostream>
#include "Config.hpp"
#include <string>
#include <iostream>
#include <fstream>

Config::Config(){}


Config &
Config::operator=(Config &&other)
{
	if (this != &other)
	{
		this = other;
	}
	return (*this);
}

// Config::Error::Error(std::string const &message)
// 	:	_message(message)
// {}

// const char *
// Config::Error::what()
// const throw() {
// 	return (_message.c_str());
// }

void
Config::getFileContent(std::string fileName){
	if (fileName.size() >= 5 && fileName.rfind(".conf") != fileName.size() - 5)
		std::cout << "Error: incorrect file extension\n"; //return, error, exit?
	std::ifstream file(fileName);
	if (!file.is_open()){
		std::cout << "Error: couldn't open file\n"; //return, error, exit?
	}
	std::string line = "";
	std::vector<Token> tokens;
	int lineNbr = 0;
	while (std::getline(file, line)){
		lineNbr++;

		std::stringstream ss(line);
		std::string word;

		while (ss >> word){
			if (word[0] == '#')
				break;
			if (!word.empty() && word.back() == ';') {
				word.pop_back();
				if (!word.empty()) 
					tokens.push_back({word, lineNbr});
				tokens.push_back({";", lineNbr});
			}
			else
				tokens.push_back({word, lineNbr});
		}
	}
	printTokens(tokens);
	TokenStream ts(tokens);
	loadFromFile(ts);
	printConfig();
}

void
Config::loadFromFile(TokenStream &ts){
	while (!ts.atEnd() && ts.current().text != "}"){
		if (ts.current().text == "Server")
			servers.push_back(parseServer(ts));
	}
	expect("}");
}

Config::Server
Config::parseServer(TokenStream &ts){
	Config::Server server;
	while (!ts.atEnd() && ts.current().text != "}"){
		if (ts.current().text == "error_page")
			server.errorPages.push_back(parseErrorPage(ts));
		if (ts.current().text == "location")
			server.locations.push_back(parseLocation(ts));
		else {
			LOG("[Config Error] Line " << ts.current().lineNbr << ": \"" << ts.getLine() << "\" -> Unknown directive");
			ts.setIndex(ts.lastTokenOnLine() + 1); //check if this works correctly
		}
	}
	expect("}");
	return (server);
}

Config::Server::ErrorPage
Config::parseErrorPage(TokenStream &ts){
	Config::Server::ErrorPage errorPage;
	ts.next();
	errorPage.code = std::stoi(ts.takeToken());
	errorPage.path = ts.takeToken();
	ts.checkSemicolon();
	return (errorPage);
}