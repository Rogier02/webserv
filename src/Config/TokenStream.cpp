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

void
TokenStream::printTokens(std::vector<Token> tokens){
	for (size_t i = 0; i < tokens.size(); i++){
		std::cout << i << "\t- line: " << tokens[i].lineNbr << "\t- Text: " << tokens[i].text << "\n";
	}
}

void
checkSemicolons(TokenStream &ts){
	int	lineNbr = ts.current().lineNbr;
	size_t lastIndex = ts.position();

	for (size_t i = lastIndex + 1; i < ts.tokens.size(); ++i) {
		if (ts.tokens[i].lineNbr == lineNbr) {
			lastIndex = i; // keep updating lastIndex
		} else {
			break;
		}
	}
	if (ts.tokens[lastIndex].text != ";")
		LOG("[Config Error] Line " << ts.current().lineNbr << ": missing semicolon at the end of directive \"" << errorPage.path << "\"\n");
}