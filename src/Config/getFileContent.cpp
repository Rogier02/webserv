#include "TokenStream.hpp"
#include "Config.hpp"
#include "log.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Config	getFileContent(std::string fileName){
	if (fileName.size() >= 5 && fileName.rfind(".conf") != fileName.size() - 5)
		std::cout << "Error: incorrect file extension\n"; //return, error, exit?
	std::ifstream file(fileName);
	if (!file.is_open()){
		std::cout << "Error: couldn't open file\n"; //return, error, exit?
		return ; //give error message
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
	Config config; // moet misschien anders
	config = loadFromFile(ts);
	config.printConfig();
	return (config);
}

Config	loadFromFile(TokenStream &ts){
	Config config; // moet misschien anders

	while (!ts.atEnd()){
		if (ts.current().text == "Server")
			config.servers.push_back(parseServer(ts));
		if (ts.current().text == "}")
			return ;
	}
	return (config);
}

Config::Server	parseServer(TokenStream &ts){
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

	return (server);
}

Config::Server::ErrorPage	parseErrorPage(TokenStream &ts){
	Config::Server::ErrorPage errorPage;

	ts.next();
	errorPage.code = std::stoi(ts.takeToken());
	errorPage.path = ts.takeToken();
	ts.checkSemicolon();

	return (errorPage);
}
