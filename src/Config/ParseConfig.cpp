#include "ParseConfig.hpp"

#define CONFIGFILE 1
#define HTTPREQUEST 2

ParseConfig::ParseConfig(std::istream& stream, int streamType)
	:	_ts(stream, streamType)
{}

Config
ParseConfig::config()
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
ParseConfig::server()
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
ParseConfig::location()
{
	Config::Server::Location	location;

	location.path = consumeValue();
	expect("{");

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
ParseConfig::single(std::string& dest)
{
	dest = consumeValue();
	expect(";");
}

void
ParseConfig::single(int& dest)
{
	dest = std::stoi(consumeValue());
	expect(";");
}

void
ParseConfig::multiple(std::vector<std::string>& dest)
{
	TokenStream::Iterator	lineEnd = _ts.lineEnd();

	while (_ts.current() != lineEnd && _ts.peek().text != ";")
		dest.push_back(consumeValue());

	expect(";");
}

void
ParseConfig::page(Config::Server::Page &page)
{
	page.code	= std::stoi(consumeValue());
	page.path	= consumeValue();
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
	std::string	number	= consumeValue();
	char		unit	= number.back();

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
ParseConfig::listen(std::string& host, int& port)
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
ParseConfig::autoIndex(bool& autoIndex)
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

// ParseConfig::expect(std::string const &expected, int lineNbr)
void
ParseConfig::expect(std::string const &expected)
{
	if (_ts.atEnd())
		log(unexpected(expected, "EOF"));
	// if (lineNbr < _ts.peek().lineNbr){
	// 	log("Unexpected amount of tokens on line " + lineNbr);
	// 	return ;
	// }
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
ParseConfig::unknownDirective(const std::string& directive)
{
	return("Unknown directive: \"" + directive + "\"" +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}

std::string
ParseConfig::unexpected(std::string const &expected, std::string const &found)
{
	return("Unexpected token: \"" + found + "\"" + " (expected: " + expected + ")" +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}

std::string
ParseConfig::consumeValue()
{
	if (_ts.peek().text == ";")
	{
		log(unexpected("no semicolon", ";"));
		return ("semicolon");
	}
	else
		return (_ts.consume());
}
