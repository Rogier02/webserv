#include "TokenStream.hpp"
#include "log.hpp"
#include <iostream>

TokenStream::TokenStream(const std::vector<Token> &t)
	:	tokens(t)
	,	index(0) {}

TokenStream::~TokenStream() {}

const Token &
TokenStream::current() const{
	return tokens[index]; 
}

void
TokenStream::next(){
	if (index < tokens.size())
		index++; 
}

bool
TokenStream::atEnd() const{
	return index >= tokens.size(); 
}

size_t
TokenStream::position() const{
	return index; 
}

size_t
TokenStream::firstTokenOnLine() const{
	int lineNbr = current().lineNbr;
	size_t first = index;

	while (first > 0 && tokens[first - 1].lineNbr == lineNbr) {
		first--;
	}

	return (first);
}

size_t
TokenStream::lastTokenOnLine() const{
	int lineNbr = current().lineNbr;
	size_t last = index;

	while (last + 1 < tokens.size() && tokens[last + 1].lineNbr == lineNbr) {
		last++;
	}

	return (last);
}

std::string
TokenStream::getLine() const{
	size_t first = firstTokenOnLine();
	size_t last = lastTokenOnLine();
	std::string line;

	for (size_t i = first; i <= last; ++i)
        line += tokens[i].text;

	return (line);
}

void
TokenStream::printTokens(std::vector<Token> tokens){
	for (size_t i = 0; i < tokens.size(); i++){
		std::cout << i << "\t- line: " << tokens[i].lineNbr << "\t- Text: " << tokens[i].text << "\n";
	}
}

void
TokenStream::checkSemicolon(TokenStream &ts){
	std::string	line;
	int			lineNbr = ts.current().lineNbr;
	size_t		i = ts.position();
	while (lineNbr == ts.current().lineNbr){
		line = ts.getLine();
		ts.next();
	}
	if (ts.current().text != ";")
		LOG("[Config Error] Line " << ts.current().lineNbr << ": missing semicolon at the end of directive >> \"" << line << "\"\n");
}