#include "Config.hpp"

// temporary
Config::Server::Location	location(TokenStream &ts);

Config::Error::Error(std::string const &message)
	:	_message(message)
{}

const char	*Config::Error::what() const throw() {
	return (_message.c_str());
}

Config::Config(std::string const &filePath)
	:	_tokens(filePath)
{
	while (!_tokens.atEnd() && _tokens.current().text != "}")
	{
		if (_tokens.current().text == "Server")
			servers.push_back(Server(_tokens));
	}
	_tokens.expect("}");
}

Config::Server::Server(TokenStream &tokens)
{
	while (!tokens.atEnd() && tokens.current().text != "}")
	{
		if (tokens.current().text == "error_page")
			errorPages.push_back(ErrorPage(tokens));
		if (tokens.current().text == "location")
			locations.push_back(location(tokens));
		else {
			LOG("[Config Error] Line " << tokens.current().lineNbr << ": \"" << tokens.getLine() << "\" -> Unknown directive");
			tokens.setIndex(tokens.lastTokenOnLine() + 1); //check if this works correctly
		}
	}
	tokens.expect("}");
}

Config::Server::ErrorPage::ErrorPage(TokenStream &tokens)
{
	tokens.next();
	code = std::stoi(tokens.takeToken());
	path = tokens.takeToken();
	tokens.checkSemicolon();
}
