#include <string>
#include <iostream>
#include <ifstream>


const std::string ReadConfigFile(std::string fileName){
	std::ifstream file = fileName;
	if (!file.isopen)
		return -1;
	std::string line;
	std::string fileContent;
	int lineNbr = 0;
	while (std::getline(file, line)){
		fileContent += line;
	}
	
#include "Config.hpp"
#include <string>
#include <iostream>
#include <fstream>

Config::Config()
{

}

Config::Config(std::string fileName)
{

}

Config &
Config::operator=(Config &&other)
{
	if (this != &other)
	{

	}
	return (*this);
}

Config::Error::Error(std::string const &message)
	:	_message(message)
{}

const char *
Config::Error::what()
const throw() {
	return (_message.c_str());
}
