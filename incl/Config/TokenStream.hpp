#ifndef TOKENSTREAM_HPP
# define TOKENSTREAM_HPP

// C++
# include <vector>
# include <string>
# include <fstream>
# include <sstream>

struct	Token
{
	std::string	text;
	int			lineNbr;
};

class	TokenStream
{
	public:
		TokenStream() = delete;
		TokenStream(TokenStream const &other) = delete;
		TokenStream(TokenStream &&other) = delete;
		TokenStream(std::string const &filePath);
		~TokenStream() = default;

	private:
		std::vector<Token>	_tokens;
		size_t				_index;

	public:
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