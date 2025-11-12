#ifndef TOKENSTREAM_HPP
#define TOKENSTREAM_HPP

#include <vector>
#include <string>

struct	Token
{
	std::string	text;
	int			lineNbr;
};

class	TokenStream
{
private:
size_t						index;
public:
TokenStream(const std::vector<Token> &t);
~TokenStream();

	const std::vector<Token>	&tokens;
	const Token &current() const;
	void next();
	bool atEnd() const;
	size_t position() const;
};



#endif