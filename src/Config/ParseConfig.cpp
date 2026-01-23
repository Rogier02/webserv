#include "ParseConfig.hpp"

#define CONFIGFILE 1
#define HTTPREQUEST 2

ParseConfig::ParseConfig(std::istream &stream, int streamType)
	:	_ts(stream, streamType)
{}

Config
ParseConfig::config()
{
	Config	config;

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.consume();
			// std::cout << directive << "\n";

		if (directive == "Server")
			config.servers.push_back(server());
		else {
			log(unknownDirective(directive));
			_ts.advanceUntil("}");
		}
	}
	// std::cout << config;

	report();
	return (config);
}

Config::Server
ParseConfig::server()
{
	Config::Server	server;

	expect("{");
	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.peek().text;
			// std::cout << directive << "\n";

		DirectiveMapIterator<ServerDirective> it = serverDirectives.find(directive);
		if (it != serverDirectives.end())
			it->second(server);
		else {
			log(unknownDirective(directive));
			_ts.advanceLine();
		}
	}
	expect("}");

	return (server);
}

Config::Server::Location
ParseConfig::location()
{
	Config::Server::Location	location;

	(void)_ts.consume();
	location.path = _ts.consume();
	expect("{");

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string	directive = _ts.peek().text;
			std::cout << directive << "\n";

		DirectiveMapIterator<LocationDirective> it = locationDirectives.find(directive);
		if (it != locationDirectives.end())
			it->second(location);
		else {
			log(unknownDirective(directive));
			_ts.advanceLine();
		}
	}
	expect("}");

	return (location);
}

void
ParseConfig::single(std::string &dest)
{
	if (!dest.empty())
	{
		doubleDirective();
		_ts.advanceLine();
		return ;
	}
	if (format(1) == -1)
		return ;

	dest = _ts.consume();
	expect(";");
}

void
ParseConfig::single(int &dest)
{
	if (dest != -1)
	{
		doubleDirective();
		_ts.advanceLine();
		return ;
	}
	if (format(1) == -1)
		return ;

	dest = std::stoi(_ts.consume());
	expect(";");
}

void
ParseConfig::multiple(std::vector<std::string> &dest)
{
	if (!dest.empty())
	{
		doubleDirective();
		_ts.advanceLine();
		return ;
	}
	if (format() == -1)
		return ;

	TokenStream::Iterator	lineEnd = _ts.lineEnd();

	while (_ts.current() != lineEnd && _ts.peek().text != ";")
		dest.push_back(_ts.consume());

	expect(";");
}

void
ParseConfig::page(Config::Server::Page &page)
{
	if (format(2) == -1)
		return ;

	page.code	= std::stoi(_ts.consume());
	page.path	= _ts.consume();
	expect(";");
}

Config::Server::Page
ParseConfig::page()
{
	Config::Server::Page	object;

	page(object);
	return (object);
}

void
ParseConfig::clientMaxBodySize(size_t &clientMaxBodySize)
{
	if (clientMaxBodySize != 0)
	{
		doubleDirective();
		_ts.advanceLine();
		return ;
	}
	if (format(1) == -1)
		return ;

	std::string	lineNbr	= std::to_string(_ts.peek().lineNbr);
	std::string	number	= _ts.consume();
	char		unit	= number.back();

	if (isalpha(unit))
		number = number.substr(0, number.size() - 1);
	else
		unit = 0;

	clientMaxBodySize = std::stoul(number);

	// default case log error
	switch(std::tolower(unit)) {
		case 'k': clientMaxBodySize *= 1024;				break;
		case 'm': clientMaxBodySize *= 1024 * 1024;			break;
		case 'g': clientMaxBodySize *= 1024 * 1024 * 1024;	break;
		default:
			if (!isdigit(unit)) {
				log("Unknown unit \"" + std::string(1, unit) + "\" on line " + lineNbr);
				return ;
			}
			break;
	}

	expect(";");
}

void
ParseConfig::listen(std::string &host, int &port)
{
	if (!host.empty() && port != 0)
	{
		log(doubleDirective());
		_ts.advanceLine();
		return ;
	}
	if (format(1) == -1)
		return ;

	std::string	hostPort	= _ts.consume();
	size_t		colonPos	= hostPort.find(':');

	if (colonPos == std::string::npos)
		log("listen directive requires host:port format");

	host	= hostPort.substr(0, colonPos);
	port	= std::stoi(hostPort.substr(colonPos + 1));
	expect(";");
}

void
ParseConfig::autoIndex(AutoIndexState &autoIndex)
{
	if (autoIndex != UNSET)
	{
		log(doubleDirective());
		_ts.advanceLine();
		return ;
	}
	if (format(1) == -1)
		return ;

	std::string	value = _ts.consume();

	if (value == "off")
		autoIndex = OFF;
	else if (value == "on")
		autoIndex = ON;
	else
		log("autoindex must be 'on' or 'off', got: " + value);

	expect(";");
}

int
ParseConfig::format(size_t noValuesExpected)
{
	int			lineNbr			= (*_ts.current()).lineNbr;
	std::string	directive		= _ts.consume();
	size_t		noValuesFound	= 0;

	TokenStream::Iterator	iterator = _ts.current();
	while (iterator != _ts.end()
		&& (*iterator).text	!= ";"
		&& (*iterator).lineNbr == lineNbr) {
		++noValuesFound;
		++iterator;
	}

	if ((*iterator).text != ";")
	{
		log("Directive " + directive + " on line " + std::to_string(lineNbr) + " is missing a semi-colon");
		if (_ts.peek().lineNbr == lineNbr)
			_ts.advanceLine();
		return (-1);
	}

	if (noValuesFound < 1)
	{
		log("Directive " + directive + " on line " + std::to_string(lineNbr) + " has no value");
		if (_ts.peek().lineNbr == lineNbr)
			_ts.advanceLine();
		return (-1);
	}

	if (noValuesExpected != 0
	&&	noValuesExpected != noValuesFound)
	{
		log("Directive " + directive + " on line " + std::to_string(lineNbr) + " has unexpected number of values: " + std::to_string(noValuesFound) + "/" + std::to_string(noValuesExpected));
		if (_ts.peek().lineNbr == lineNbr)
			_ts.advanceLine();
		return (-1);
	}

	return (noValuesFound);
}

void
ParseConfig::expect(std::string const &expected)
{
	if (_ts.atEnd())
		log(unexpected(expected, "EOF"));

	std::string	token = _ts.peek().text;
	if (token != expected)
		log(unexpected(expected, token));
	_ts.advance();
}

void
ParseConfig::log(std::string const &message) {
	_log.push_back(message);
}

void
ParseConfig::report()
{
	if (_log.empty())
		return ;

	LOGGER(startBlock("Config File Errors"));
	for (std::string message : _log)
		LOGGER(log(message));
	LOGGER(endBlock());

	throw std::runtime_error(std::string("Config File Errors detected, see \"") + Logger::FileName + "\" for detailed error messages");
}

std::string
ParseConfig::unknownDirective(const std::string &directive)
{
	return("Unknown directive: \"" + directive + "\"" +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}

std::string
ParseConfig::doubleDirective()
{
	return ("Directive on line " + std::to_string(_ts.peek().lineNbr) + " has already been set on a previous line.");
}

std::string
ParseConfig::noValueFound(std::string &directive, int lineNbr)
{
	return ("Directive " + directive + " on line " + std::to_string(lineNbr) + " has no value");
}

std::string
ParseConfig::noSemicolon(std::string &directive, int lineNbr)
{
	return ("Directive " + directive + " on line " + std::to_string(lineNbr) + " is missing a semi-colon");
}

std::string
ParseConfig::unexpectedNoValues(std::string &directive, int lineNbr, int noValuesFound, int noValuesExpected)
{
	return ("Directive " + directive + " on line " + std::to_string(lineNbr) + " has unexpected number of values: " + std::to_string(noValuesFound) + "/" + std::to_string(noValuesExpected));
}

std::string
ParseConfig::unexpected(std::string const &expected, std::string const &found)
{
	return("Unexpected token: \"" + found + "\"" + " (expected: " + expected + ")" +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}