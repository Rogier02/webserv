#include "Parse.hpp"

Parse::Parse(std::string const &filePath)
	:	_ts(filePath)
{}

Config
Parse::config()
{
	Config	config;

	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.consume();

		if (directive == "Server")
			config.listeners.push_back(server());
		else {
			log(unknownDirective(directive));
			_ts.advanceToBracket();
		}
	}

	report();
	return (config);
}

Config::Listener
Parse::server()
{
	Config::Listener		server;

	std::set<std::string>								directives;
	std::pair<std::set<std::string>::iterator, bool>	result;

	expect("{");
	while (!_ts.atEnd() && _ts.peek().text != "}")
	{
		std::string directive = _ts.consume();

		result = directives.insert(directive);
		if (!result.second && directive != "location" && directive!= "error_page") {
			log(unexpectedMessage("Duplicate directive", directive));
			_ts.advanceLine();
		}
		else
		{
			if (serverDirectives.contains(directive))
				serverDirectives.at(directive)(server);
			else {
				log(unknownDirective(directive));
				_ts.advanceLine();
			}
		}
	}
	expect("}");
	return (server);
}

void
Parse::errorPage(std::map<u_int16_t, std::string> &dest)
{
	::size_t	tokensFound = _ts.tokensOnLine();

	std::pair<std::map<u_int16_t, std::string>::iterator, bool>	result;

	if (tokensFound == 4)
	{
		std::string directive = _ts.consume();
		u_int16_t	code;

		if (!isAllDigits(directive))
		{
			log(unexpectedMessage("Code can only contain 0-9", directive));
			_ts.advanceLine();
		}
		else
		{
			code = std::stoi(directive);

			std::string	path = _ts.consume();

			result = dest.insert(std::make_pair(code, path));
			if (!result.second) {
				log(unexpectedMessage("Duplicate code", std::to_string(code)));
			}

			expect(";");
		}
	}
	else {
		log(unexpectedTokenCount("4", tokensFound));
		_ts.advanceLine();
	}
}

void
Parse::location(Config::Listener &server)
{
	::size_t	tokensFound = _ts.tokensOnLine();

	std::set<std::string>								directives;
	std::pair<std::set<std::string>::iterator, bool>	result;

	if (tokensFound == 3) {
		Config::Listener::Location	location;
		std::string					path = _ts.consume();

		if (server.locations.contains(path)) {
			log("Duplicate location: " + path);
			_ts.advanceTillBracket();
		}

		location.root				= server.root;
		location.clientMaxBodySize	= server.clientMaxBodySize;

		expect("{");
		while (!_ts.atEnd() && _ts.peek().text != "}")
		{
			std::string	directive = _ts.consume();
			result = directives.insert(directive);
			if (!result.second) {
				log(unexpectedMessage("Duplicate directive", directive));
				_ts.advanceLine();
			}
			else
			{
				if (locationDirectives.contains(directive))
					locationDirectives.at(directive)(location);
				else {
					log(unknownDirective(directive));
					_ts.advanceLine();
				}
			}
		}
		server.locations[path] = location;

		expect("}");
	}
	else {
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceToBracket();
		std::cout << _ts.peek().text << std::endl;
	}
}

void
Parse::single(std::string &dest)
{
	::size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		dest += _ts.consume();
		expect(";");
	}
	else {
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceLine();
	}
}

void
Parse::single(u_int16_t &dest)
{
	::size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		dest = std::stoi(_ts.consume());
		expect(";");
	}
	else {
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceLine();
	}
}

void
Parse::multiple(std::string &dest)
{
	TokenStream::Iterator	lineEnd = _ts.lineEnd();
	::size_t				tokensFound = _ts.tokensOnLine();

	if (tokensFound >= 3) {
		while (_ts.current() != lineEnd && _ts.peek().text != ";")
			dest += _ts.consume() + ' ';
		if (dest.back() == ' ')
			dest.pop_back();

		expect(";");
	}
	else {
		log(unexpectedTokenCount("3 or more", tokensFound));
		_ts.advanceLine();
	}
}

void
Parse::multiple(std::set<std::string> &dest)
{
	TokenStream::Iterator	lineEnd = _ts.lineEnd();
	::size_t				tokensFound = _ts.tokensOnLine();

	std::pair<std::set<std::string>::iterator, bool>	result;

	if (tokensFound >= 3) {
		while (_ts.current() != lineEnd && _ts.peek().text != ";")
		{
			std::string token = _ts.consume();
			result = dest.insert(token);
			if (!result.second)
				log(unexpectedMessage("Duplicate method", token));
		}

		expect(";");
	}
	else {
		log(unexpectedTokenCount("3 or more", tokensFound));
		_ts.advanceLine();
	}
}

void
Parse::listen(std::string &host, int &port)
{
	::size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		std::string	hostPort	= _ts.consume();
		::size_t	colonPos	= hostPort.find(':');

		if (isValidHostChars(hostPort))
		{
			if (colonPos == std::string::npos)
				log(unexpectedMessage("Listen directive requires host:port format", hostPort));

			host	= hostPort.substr(0, colonPos);
			port	= std::stoi(hostPort.substr(colonPos + 1));
		}
		else
			log(unexpectedMessage("Can only contain 0-9, . or :", hostPort));
		expect(";");
	}
	else {
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceLine();
	}
}

bool
Parse::isValidHostChars(const std::string& str)
{
	if (str.empty())
		return (false);

	for (::size_t i = 0; i < str.size(); i++)
		if (!std::isdigit((unsigned char)str[i]) && str[i] != '.' && str[i] != ':')
			return (false);

	return (true);
}

void
Parse::clientMaxBodySize(::size_t &clientMaxBodySize)
{
	::size_t	tokensFound = _ts.tokensOnLine();
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
	else {
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceLine();
	}
}

void
Parse::returnPage(u_int16_t &code, std::string &path)
{
	::size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 4) {
		code = std::stoi(_ts.consume());
		path = _ts.consume();

		expect(";");
	}
	else {
		log(unexpectedTokenCount("4", tokensFound));
		_ts.advanceLine();
	}
}

void Parse::autoIndex(bool &autoIndex)
{
	::size_t	tokensFound = _ts.tokensOnLine();
	if (tokensFound == 3) {
		std::string	value = _ts.consume();

		if (value == "off")
			autoIndex = false;
		else if (value == "on")
			autoIndex = true;
		else
			log(unexpected("autoindex must be 'on' or 'off'", value));

		expect(";");
	}
	else {
		log(unexpectedTokenCount("3", tokensFound));
		_ts.advanceLine();
	}
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

// bool
// Parse::duplicate(std::string const &directive, std::string const &message)
// {
// 	std::set<std::string>								directives;
// 	std::pair<std::set<std::string>::iterator, bool>	result;

// 	if (directive != "location" && directive!= "error_page")
// 	{
// 		result = directives.insert(directive);
// 		if (!result.second) {
// 			log(unexpectedMessage(message, directive));
// 			_ts.advanceLine();
// 			return (true);
// 		}
// 	}
// 	return (false);
// }

bool
Parse::isAllDigits(std::string const &str)
{
	if (str.empty())
		return (false);

	for (::size_t i = 0; i < str.size(); i++)
		if (!std::isdigit((unsigned char)str[i]))
			return (false);

	return (true);
}

void Parse::log(std::string const &message) {
	_log.push_back(message);
}

void Parse::report()
{
	if (_log.empty())
		return ;

	for (std::string message : _log)
		LOG(Info, message);

	throw std::runtime_error(std::string("Config File Errors detected, see \"") + Logger::FileName + "\" for detailed error messages");
}

std::string
Parse::unknownDirective(const std::string &directive)
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
Parse::unexpectedMessage(std::string const &expected, std::string const &found)
{
	return("Unexpected: \"" + found + "\" > " + expected +
		" on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}
std::string
Parse::unexpectedTokenCount(std::string expected, ::size_t found)
{
	return ("Unexpected token count: " + std::to_string(found) + " (expected: " + expected + ")"
		+ "on line " + std::to_string(_ts.peek().lineNbr) + ": " + _ts.getLine());
}
