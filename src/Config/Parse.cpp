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
		std::string directive = consumeValue();

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
		std::string directive = consumeValue();

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

	location.path = consumeValue();
	expect("{");

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string	directive = consumeValue();

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
	dest = consumeValue();
	expect(";");
}

void
Parse::single(int& dest)
{
	dest = std::stoi(consumeValue());
	expect(";");
}

void
Parse::multiple(std::vector<std::string>& dest)
{
	TokenStream::Iterator	lineEnd = _ts.lineEnd();

	while (_ts.current() != lineEnd && _ts.peek().text != ";")
		dest.push_back(consumeValue());

	expect(";");
}

void
Parse::page(Config::Server::Page &page)
{
	page.code	= std::stoi(consumeValue());
	page.path	= consumeValue();
	expect(";");
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
	std::string	token	= _ts.peek().text;
	char		unit	= token.back();
	std::string	number	= consumeValue();

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

void
Parse::listen(std::string& host, int& port)
{
	std::string	hostPort	= consumeValue();
	size_t		colonPos	= hostPort.find(':');

	if (colonPos == std::string::npos)
		log("listen directive requires host:port format");

	host	= hostPort.substr(0, colonPos);
	port	= std::stoi(hostPort.substr(colonPos + 1));
	expect(";");
}

void
Parse::autoIndex(bool& autoIndex)
{
	std::string	value = consumeValue();

	if (value == "off")
		autoIndex = false;
	else if (value == "on")
		autoIndex = true;
	else
		log("autoindex must be 'on' or 'off', got: " + value);

	expect(";");
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
Parse::consumeValue()
{
	if (_ts.peek().text == ";")
	{
		log(unexpected("no semicolon", ";"));
		return ("semicolon");
	}
	else
		return (_ts.consume());
}