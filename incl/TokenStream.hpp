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

	const Token	&current() const;
	void		next();
	std::string	takeToken();
	bool		atEnd() const;
	size_t		position() const;
	size_t		firstTokenOnLine() const;
	size_t		lastTokenOnLine() const;
	bool		isLastTokenOnLine() const ;
	std::string	getLine() const;
	void		printTokens(std::vector<Token> tokens);
	void		checkSemicolon();
	void		setIndex(size_t newIndex);
	void		expect(std::string expected);
};

void	printTokens(std::vector<Token> tokens);


#endif