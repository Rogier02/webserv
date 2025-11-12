#include "TokenStream.hpp"

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
