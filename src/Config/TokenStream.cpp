#include "TokenStream.hpp"
#include "log.hpp"

TokenStream::TokenStream(std::string const &fileName)
	:	_index(0)
{
	if (fileName.rfind(".conf") == std::string::npos)
		LOG("incorrect file extension\n");

	std::ifstream	file(fileName);
	if (!file.is_open())
		LOG("couldn't open file\n");

	std::string	line;
	int			lineNbr = 1;
	while (std::getline(file, line))
	{
		std::stringstream	stream(line);
		std::string			word;

		while (stream >> word)
		{
			if (word[0] == '#')
				break;
			if (!word.empty() && word.back() == ';')
			{
				word.pop_back();
				if (!word.empty())
					_tokens.push_back(Token{word, lineNbr});
				_tokens.push_back(Token{";", lineNbr});
			}
			else
				_tokens.push_back(Token{word, lineNbr});
		}
		++lineNbr;
	}
	file.close();
}

const Token &
TokenStream::current()
const {
	return _tokens[_index];
}

void
TokenStream::next()
{
	if (_index < _tokens.size())
		_index++;
}

std::string
TokenStream::takeToken()
{
	std::string text = current().text;
	next();
	return (text);
}


bool
TokenStream::atEnd()
const {
	return _index >= _tokens.size();
}

size_t
TokenStream::position()
const {
	return _index;
}

size_t
TokenStream::firstTokenOnLine()
const {
	int lineNbr = current().lineNbr;
	size_t first = _index;

	while (first > 0 && _tokens[first - 1].lineNbr == lineNbr) {
		first--;
	}

	return (first);
}

size_t
TokenStream::lastTokenOnLine()
const {
	int lineNbr = current().lineNbr;
	size_t last = _index;

	while (last + 1 < _tokens.size() && _tokens[last + 1].lineNbr == lineNbr) {
		last++;
	}

	return (last);
}

bool
TokenStream::isLastTokenOnLine()
const {
	return (position() == lastTokenOnLine());
}

std::string
TokenStream::getLine()
const {
	size_t first = firstTokenOnLine();
	size_t last = lastTokenOnLine();
	std::string line;

	for (size_t i = first; i <= last; ++i)
        line += _tokens[i].text;

	return (line);
}

void
TokenStream::printTokens(std::vector<Token> _tokens)
{
	for (size_t i = 0; i < _tokens.size(); i++){
		std::cout << i << "\t- line: " << _tokens[i].lineNbr << "\t- Text: " << _tokens[i].text << "\n";
	}
}

void
TokenStream::checkSemicolon()
{
	std::string	line;

	line = getLine();
	if (current().text != ";")
		LOG("[Config Error] at line: " << current().lineNbr << ": missing semicolon at the end of directive >> \"" << line << "\"\n");
	// else
	next(); //this might not work. Test when ready
}

void
TokenStream::setIndex(size_t newIndex)
{
	if (newIndex < _tokens.size()) {
		_index = newIndex;
	}
	else {
		LOG("[Set Index Error] couldn't set _index.\n");
	}
}

void
TokenStream::expect(std::string expected)
{
	if (current().text != expected){
		LOG("[Config Error] at line: " << current().lineNbr << " \"" << expected << "\" was expected, but not found.\n");
		if (!atEnd())
			setIndex(lastTokenOnLine() + 1);
	}
	else
		next(); //check if this works. Don't wanna test it now.
}
