#include "Parse.hpp"

Parse::Parse(std::string const &filePath)
	:	_ts(filePath)
{}

Config Parse::config()
{
	Config	config;

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.consume();

		if (directive == "Server")
			config.servers.push_back(server());
		else {
			log(unknownDirective(directive));
			_ts.advanceTillBracket();
		}
	}
	expect('}');

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

	if (_ts.tokensOnLine() == 3){
		location.path = _ts.consume();
		expect("{");
	}
	else{
		log()
		_ts.advanceLine();
	}

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
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
	dest = _ts.consume();
	expect(";");
}

void
Parse::single(int& dest)
{
	dest = std::stoi(_ts.consume());
	expect(";");
}

void
Parse::multiple(std::vector<std::string>& dest)
{
	TokenStream::Iterator	lineEnd = _ts.lineEnd();

	while (_ts.current() != lineEnd && _ts.peek().text != ";")
		dest.push_back(_ts.consume());

	expect(";");
}

void
Parse::page(Config::Server::Page &page)
{
	size_t	tokensOnLine = _ts.tokensOnline();
	if (tokensOnline == 4) {
		page.code	= std::stoi(_ts.consume());
		page.path	= _ts.consume();
		expect(";");
	}
	else
		log(unexpectedTokenCount(4, tokensOnLine));
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
	size_t	tokensOnLine = _ts.tokensOnline();
	if (tokensOnline == 3) {
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
		log(unexpectedTokenCount(3, tokensOnLine));
}

void
Parse::listen(std::string& host, int& port)
{
	size_t	tokensOnLine = _ts.tokensOnline();
	if (tokensOnline == 3) {
		std::string	hostPort	= _ts.consume();
		size_t		colonPos	= hostPort.find(':');

		if (colonPos == std::string::npos)
			log("listen directive requires host:port format");

		host	= hostPort.substr(0, colonPos);
		port	= std::stoi(hostPort.substr(colonPos + 1));
		expect(";");
	}
	else
		log(unexpectedTokenCount(3, tokensOnLine));
}

void Parse::autoIndex(bool& autoIndex)
{
	size_t	tokensOnLine = _ts.tokensOnline();
	if (tokensOnline == 3) {
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
		log(unexpectedTokenCount(3, tokensOnLine));
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

void Parse::log(std::string const &message) {
	_log.push_back(message);
}

void Parse::report()
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
Parse::unexpectedTokenCount(size_t expected, size_t found)
{
	return ("Unexpected token count: " + std::to_string(found) + " (expected: " + std::to_string(expected) + ") on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}
