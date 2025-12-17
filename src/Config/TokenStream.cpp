#include "TokenStream.hpp"
#include "log.hpp"

#define CONFIGFILE 1
#define HTTPREQUEST 2

TokenStream::TokenStream(std::istream& stream, int streamType)
{
	std::string	line;
	int			lineNbr = 1;
	while (std::getline(stream, line))
	{
		std::stringstream	stream(line);
		std::string			word;
		bool				skipline = false;

		while (!skipline && stream >> word)
		{
			switch (streamType)
			{
				case CONFIGFILE:
					if (word[0] == '#'){
						skipline = true;
						break;
					}
					if (!word.empty() && word.back() == ';')
					{
						word.pop_back();
						if (!word.empty())
							_tokens.push_back(Token{word, lineNbr});
						_tokens.push_back(Token{";", lineNbr});
					}
					else
						_tokens.push_back(Token{word, lineNbr});
					break;
				case HTTPREQUEST:
					if (!word.empty() && (word.back() == '\r' || word.back() == ':')) // Maybe check if \r en : wel echt delete moet worden.
					{
						word.pop_back();
						if (!word.empty())
							_tokens.push_back(Token{word, lineNbr});
					}
					else
						_tokens.push_back(Token{word, lineNbr});
					break;
			}
		}
		++lineNbr;
	}
	_current = _tokens.begin();
}

TokenStream::Iterator TokenStream::current() const {
	return (_current);
}

TokenStream::Iterator TokenStream::begin() const {
	return (_tokens.begin());
}

TokenStream::Iterator TokenStream::end() const {
	return (_tokens.end());
}

TokenStream::Token const &TokenStream::peek() const {
	return (*_current);
}

std::string TokenStream::consume() {
	return ((_current++)->text);
}

void TokenStream::advance() {
	++_current;
}

bool TokenStream::atEnd() const {
	return (_current >= _tokens.end());
}

TokenStream::Iterator
TokenStream::lineStart()
const {
	int			lineNbr	= peek().lineNbr;
	Iterator	it		= _current;

	while (it != _tokens.begin() && (it - 1)->lineNbr == lineNbr)
		--it;

	return (it);
}

TokenStream::Iterator
TokenStream::lineEnd()
const {
	int			lineNbr	= peek().lineNbr;
	Iterator	it		= _current;

	while (it != _tokens.end() && it->lineNbr == lineNbr)
		++it;

	return (it);
}

std::string
TokenStream::getLine()
const {
	Iterator	start	= lineStart();
	Iterator	end		= lineEnd();

	std::string	line;
	for (Iterator it = start; it != end; ++it)
		line += it->text + " ";

	if (!line.empty())
		line.pop_back();

	return (line);
}

void
TokenStream::advanceLine() {
	_current = lineEnd();
}

void
TokenStream::advanceUntil(const std::string& target) {
	while (!atEnd() && _current->text != target)
		advance();
}

std::size_t
TokenStream::tokensOnLine() {
	Iterator	start	= lineStart();
	Iterator	end		= lineEnd();

	return(static_cast<std::size_t>(end - start));
}