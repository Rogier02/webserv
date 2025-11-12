#include "TokenStream.hpp"
#include "Config.hpp"
#include "log.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void	printTokens(std::vector<Token> tokens){
	for (size_t i = 0; i < tokens.size(); i++){
		std::cout << i << "\t- line: " << tokens[i].lineNbr << "\t- Text: " << tokens[i].text << "\n";
	}
}

void	logConfigError(const Token &token, const std::string &message){
	std::cerr << "[Config Error] Line " << token.text << ": \"" << token.text << "\" -> " << message << std::endl;
}

void	getFileContent(std::string fileName){
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
			if (line.empty() || word[0] == '#')
				break;
			tokens.push_back({word, lineNbr});
		}
	}
	printTokens(tokens);
	TokenStream ts(tokens);
	// now i have a lot of tokens.
	// Next: do something with them/
	Config config();
	config.loadFromFile(&ts);
}

void	loadFromFile(TokenStream &ts){
	while (!ts.atEnd()){
		if (ts.current().text == "Server")
			config.servers.push_back(parseServer(ts));
		if (ts.current().text == "}")
			return ;
	}
}

Config::Server	parseServer(TokenStream &ts){
	Config::Server server;
	ts.next();
	if (ts.current().text == "error_page")
		config.errorPages.push_back(parseErrorPage(ts));
	if (ts.current().text == "location")
		config.locations.push_back(parseLocation(ts));
	else {
		LOG("[Config Error] Line " << ts.current().text << ": \"" << ts.current().text << "\" -> Unknown directive");
	}
	if (ts.current().text == "}")
		return (server);
}

Config::Server::ErrorPage	parseErrorPage(TokenStream &ts){
	Config::Server::ErrorPage errorPage;
	ts.next();
	errorPage.code = std::stoi(ts.current().text);
	ts.next();
	errorPage.path = ts.current().text;
	if (errorPage.path.back() != ';') {
		LOG("[Config Error] Line " << ts.current().lineNbr << ": missing semicolon at the end of directive \"" << errorPage.path << "\"\n");
	}
	else
		errorPage.path.pop_back();
	return (errorPage);
}

Config::Server::Location	parseLocation(TokenStream &ts){
	Config::Server::Location location;
	ts.next();
	location.path = ts.current().text;
	
	if (ts.current().text == "}")
		return(location);
}

int	main()
{
	getFileContent("default.conf");

	return (0);
}