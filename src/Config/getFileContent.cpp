#include "Config.hpp"


void	getFileContent(){
	if (filename.size() >= 5 && filename.rfind(".conf") != filename.size() - 5;)
		std::cout << "Error: incorrect file extension\n"; //return, error, exit
	std::ifstream file = fileName;
	if (!file.isopen)
		return -1;
	std::string line;
	std::string fileContent;
	while (std::getline(file, line)){
		fileContent += line;
	}
	int lineNbr = 0;
	

}