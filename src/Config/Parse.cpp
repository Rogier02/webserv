#include "Parse.hpp"

#define CONFIGFILE 1

// Parse::Parse(std::string const &filePath)
// 	:	_ts(filePath)
// {}

Parse::Parse(std::istream& stream, int streamType)
	:	_ts(stream, streamType)
{}

Config
Parse::config()
{
	Config	config;

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.consume();

		if (directive == "Server")
			config.servers.push_back(server());
		else {
			log(unknownDirective(directive));
			_ts.advanceUntil("}");
		}
	}

	report();
	return (config);
}

Config::Server
Parse::server()
{
	Config::Server	server;

	expect("{");
	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.consume();

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
Parse::location()
{
	Config::Server::Location	location;
	size_t						tokensFound = _ts.tokensOnLine();

	if (tokensFound == 3){
		location.path = _ts.consume();
		expect("{");
	}
	else{
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceLine();
	}

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		// Hier moet eigenlijk ook elke keer een tokenCount worden gedaan om te checken of er wel genoeg tokens in de line zitten denk ik > Of is dit al opgevangen in single() en multiple()??
		std::string	directive = _ts.consume();

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
Parse::single(std::string& dest)
{
	size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		dest = _ts.consume();
		expect(";");
	}
	else
		log(unexpectedTokenCount("3", tokensFound));
}

void
Parse::single(int& dest)
{
	size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		dest = std::stoi(_ts.consume());
		expect(";");
	}
	else
		log(unexpectedTokenCount("3", tokensFound));
}

void
Parse::multiple(std::vector<std::string>& dest)
{
	TokenStream::Iterator	lineEnd = _ts.lineEnd();
	size_t					tokensFound = _ts.tokensOnLine();
	if (tokensFound >= 3) {
		while (_ts.current() != lineEnd && _ts.peek().text != ";")
			dest.push_back(_ts.consume());
	
		expect(";");
	}
	else
		log(unexpectedTokenCount("3 or more", tokensFound));
}

void
Parse::page(Config::Server::Page &page)
{
	size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 4) {
		page.code	= std::stoi(_ts.consume());
		page.path	= _ts.consume();
		expect(";");
	}
	else
		log(unexpectedTokenCount("4", tokensFound));
}

Config::Server::Page
Parse::page()
{
	Config::Server::Page	object;

	page(object);
	return (object);
}

void
Parse::clientMaxBodySize(size_t &clientMaxBodySize)
{
	size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		std::string	token	= _ts.peek().text;
		char		unit	= token.back();
		std::string	number	= _ts.consume();

		if (unit == 'k' || unit == 'm' || unit == 'g')
			number = number.substr(0, number.size() - 1);
		else
			unit = 0;

		clientMaxBodySize = std::stoul(number);

		switch(unit) {
			case 'k': clientMaxBodySize *= 1024;				break;
			case 'm': clientMaxBodySize *= 1024 * 1024;			break;
			case 'g': clientMaxBodySize *= 1024 * 1024 * 1024;	break;
		}

		expect(";");
	}
	else
		log(unexpectedTokenCount("3", tokensFound));
}

void
Parse::listen(std::string& host, int& port)
{
	size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		std::string	hostPort	= _ts.consume();
		size_t		colonPos	= hostPort.find(':');

		if (colonPos == std::string::npos)
			log("listen directive requires host:port format");

		host	= hostPort.substr(0, colonPos);
		port	= std::stoi(hostPort.substr(colonPos + 1));
		expect(";");
	}
	else
		log(unexpectedTokenCount("3", tokensFound));
}

void
Parse::autoIndex(bool& autoIndex)
{
	size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		std::string	value = _ts.consume();

		if (value == "off")
			autoIndex = false;
		else if (value == "on")
			autoIndex = true;
		else
			log("autoindex must be 'on' or 'off', got: " + value);

		expect(";");
	}
	else
		log(unexpectedTokenCount("3", tokensFound));
}

void
Parse::expect(std::string const &expected)
{
	if (_ts.atEnd())
		log(unexpected(expected, "EOF"));

	std::string	token = _ts.peek().text;
	if (token != expected)
		log(unexpected(expected, token));
	_ts.advance();
}

void
Parse::log(std::string const &message) {
	_log.push_back(message);
}

void
Parse::report()
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
Parse::unknownDirective(const std::string& directive)
{
	return("Unknown directive: \"" + directive + "\"" +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}

std::string
Parse::unexpected(std::string const &expected, std::string const &found)
{
	return("Unexpected token: \"" + found + "\"" + " (expected: " + expected + ")" +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}

std::string
Parse::unexpectedTokenCount(std::string expected, size_t found)
{
	return ("Unexpected token count: " + std::to_string(found) + " (expected: " + expected + ") on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}

std::string
Parse::consumeValue()
{
	std::string value = _ts.consume();
	if (value == ";")
		log("Unexpected ; on line ");
	return (value);
}