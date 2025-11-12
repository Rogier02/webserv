#include "../../incl/TokenStream.hpp"
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
			serverConfig(ts.next());
		if (ts.current().text == "}")
			return ;

	}
}

void	serverConfig(TokenStream &ts){
		if (ts.current().text == "error_page")
			errorPageConfig(ts.next());
		if (ts.current().text == "location")
			locationConfig(ts.next());
		if (ts.current().text == "}")
			return ;
		else {
			logConfigError(ts.current(), "Unknown directive");
		}
		if (ts.current().text == "}")
			return ;
}

void	errorPageConfig(TokenStream &ts){

 	if (tokens[start + 1].text.back() != ';') {
		std::cerr << "[Config Error] Line " << tokens[start + 1].lineNbr << ": missing semicolon at the end of directive \"" << tokens[start + 1].text << "\"\n";
		return (-1); // or throw an exception
	}
	tokens[start + 1].text.pop_back();
	config.errorPages.push_back({tokens[start], tokens[start + 1]});
}

void	locationConfig(TokenStream &ts){

}

int	main()
{
	getFileContent("default.conf");

	return (0);
}