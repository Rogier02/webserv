#include <string>
#include <iostream>
#include <ifstream>


const char *ReadConfigFile(std::string fileName){
	std::ifstream file = fileName;
	if (!file.isopen)
		return -1;
	std::string line;
	std::string fileContent;
	int lineNbr = 0;
	while (std::getline(file, line)){
		fileContent += line;
	}
	
}
