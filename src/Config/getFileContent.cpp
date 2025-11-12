#include "Config.hpp"

void	printTokens(std::vector<Token> tokens){
	for (int i = 0; i < tokens.size(); i++){
		std::cout << i << " - Text: " << tokens[i].text << " - line: " << tokens[i].lineNbr << "\n";
	}
}


void	getFileContent(std::string fileName){
	if (fileName.size() >= 5 && fileName.rfind(".conf") != fileName.size() - 5;)
		std::cout << "Error: incorrect file extension\n"; //return, error, exit?
	std::ifstream file(fileName);
	if (!file.isopen)
		return -1; //give error message
	std::string line = nullptr;
	std::vector<Token> tokens;
	int lineNbr = 0;
	while (std::getline(file, line)){
		lineNbr++;

		std::stringstream ss(line);
		std::string word;

		while (ss >> word){
			if (line.empty() || word[0] == '#')
				continue;
			tokens.push_back({word, lineNbr});
		}
	}
	// now i have a lot of tokens.
	// Next: do something with them/

}

int	main(){
	
}