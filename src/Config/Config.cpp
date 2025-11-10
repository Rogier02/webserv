#include "Config.hpp"

Config::Config()
{

}

Config::Config(std::string file)
{
	(void)file;
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
