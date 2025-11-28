#include "Parse.hpp"

Parse::Parse(std::string const &filePath)
	:	ts(filePath)
{}

Config
Parse::config()
{
	Config		config;
	while (!ts.atEnd() && ts.current().text != "}")
		if (ts.takeToken() == "Server")
			config.servers.push_back(server());
	return (config);
}

Config::Server
Parse::server()
{
	Config::Server	server;

	ts.expect("{");
	while (!ts.atEnd() && ts.current().text != "}")
	{
		std::string directive = ts.takeToken();

		auto it = serverDirectives.find(directive);
		if (it != serverDirectives.end())
			it->second(server);
		else {
			throw UnknownDirective(directive, ts.current().lineNbr, ts.getLine());
			ts.setIndex(ts.lastTokenOnLine() + 1);
		}
	}
	ts.expect("}");
	return (server);
}

Config::Server::Location
Parse::location()
{
	Config::Server::Location location;
	location.path = ts.takeToken();

	ts.expect("{");
	while (!ts.atEnd() && ts.current().text != "}")
	{
		std::string directive = ts.takeToken();

		auto it = locationDirectives.find(directive);
		if (it != locationDirectives.end())
			it->second(location);
		else {
			throw UnknownDirective(directive, ts.current().lineNbr, ts.getLine());
			ts.setIndex(ts.lastTokenOnLine() + 1);
		}
	}
	ts.expect("}");
	return location;
}

void
Parse::single(std::string &dest)
{
	dest = ts.takeToken();
	ts.checkSemicolon();
}

void
Parse::single(int &dest)
{
	dest = std::stoi(ts.takeToken());
	ts.checkSemicolon();
}

void
Parse::multiple(std::vector<std::string> &dest)
{
	while (!ts.isLastTokenOnLine())
		dest.push_back(ts.takeToken());
	ts.checkSemicolon();
}

Config::Server::Page
Parse::page()
{
	Config::Server::Page	page;
	page.code = std::stoi(ts.takeToken());
	page.path = ts.takeToken();
	ts.checkSemicolon();
	return (page);
}

void
Parse::page(Config::Server::Page &page)
{
	page.code = std::stoi(ts.takeToken());
	page.path = ts.takeToken();
	ts.checkSemicolon();
}

void
Parse::clientMaxBodySize(size_t &clientMaxBodySize)
{
	char		unit = ts.current().text.back();
	std::string	number = ts.takeToken();

	if (unit == 'k' || unit == 'm' || unit == 'g')
		number = number.substr(0, number.size() - 1);
	else
		unit = 0;

	clientMaxBodySize = std::stoul(number);

	switch(unit) {
		case 'k': clientMaxBodySize *= 1024;
			break;
		case 'm': clientMaxBodySize *= 1024 * 1024;
			break;
		case 'g': clientMaxBodySize *= 1024 * 1024 * 1024;
			break;
	}
	ts.checkSemicolon();
}

void
Parse::listen(std::string &host, int &port)
{
	std::string	hostPort = ts.takeToken();
	size_t		colonPos = hostPort.find(':');

	if (colonPos == std::string::npos)
		throw UnknownDirective("<missing colon>", ts.current().lineNbr, ts.getLine());

	host = hostPort.substr(0, colonPos);
	port = std::stoi(hostPort.substr(colonPos + 1));
	ts.checkSemicolon();
}

void
Parse::autoIndex(bool &autoIndex)
{
	std::string current = ts.takeToken();

	if (current == "off")
		autoIndex = false;
	else if (current == "on")
		autoIndex = true;
	else
		throw UnknownDirective(current, ts.current().lineNbr, ts.getLine());
	ts.checkSemicolon();
}

Parse::UnknownDirective::UnknownDirective(
	const std::string& directive,
	int lineNbr,
	const std::string& line)
	:	MessageException(
		"[Config Error] Unknown directive: \"" + directive + "\"" +
		" on line " + std::to_string(lineNbr) + ": " + line)
{}
