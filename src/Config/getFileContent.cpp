#include "../../incl/Config.hpp"
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
	// now i have a lot of tokens.
	// Next: do something with them/
	Config config();
	config.loadFromFile(tokens);
}

void	loadFromFile(std::vector<Token> tokens){
	for (size_t i = 0; i < tokens.size(); i++){
		if (tokens[i].text == "Server")
			serverConfig(tokens, i);
		if (tokens[i].text == "}")
			return ;
	}
}

void	serverConfig(std::vector<Token> tokens, size_t start){
	for (size_t i = start; i < tokens.size(); i++){
		if (tokens[i].text == "error_page")
			errorPagesConfig(tokens, i);
		if (tokens[i].text == "}")
			return ;
		else {
			std::cout << 
		}
	}
}

int	main()
{
	getFileContent("default.conf");

	return (0);
}