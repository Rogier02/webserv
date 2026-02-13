#include "IO.hpp"

std::string
readFile(std::string const &filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		return ("");
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return (buffer.str());
}
