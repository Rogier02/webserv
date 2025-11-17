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

std::string
TokenStream::takeToken(){
	std::string text = current().text;
	next();
	return (text);
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

bool
TokenStream::isLastTokenOnLine() const{
	return (position() == lastTokenOnLine());
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
TokenStream::checkSemicolon(){
	std::string	line;

	line = getLine();
	if (current().text != ";")
		LOG("[Config Error] at line: " << current().lineNbr << ": missing semicolon at the end of directive >> \"" << line << "\"\n");
	else
		next(); //this might not work. Test when ready
}

void
TokenStream::setIndex(size_t newIndex){
	if (newIndex < tokens.size()) {
		index = newIndex;
	}
	else {
		LOG("[Set Index Error] couldn't set index.\n");
	}
}

void
TokenStream::expect(std::string expected){
	if (current().text != expected){
		LOG("[Config Error] at line: " << current().lineNbr << " \"" << expected << "\" was expected, but not found.\n");
		if (!atEnd())
			setIndex(isLastTokenOnLine() + 1);
	}
	else
		next(); //check if this works. Don't wanna test it now.
}