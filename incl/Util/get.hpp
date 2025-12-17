#ifndef GET_HPP
#define GET_HPP

#include <string>
#include <fstream>



std::ifstream	getFile(std::string configFilePath);
std::ifstream	getHttpRequest(int clientFd);


#endif