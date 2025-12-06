#ifndef TOKENSTREAM_HPP
# define TOKENSTREAM_HPP

// C++
# include <vector>
# include <string>
# include <fstream>
# include <sstream>
# include <algorithm>

class	TokenStream
{
	public:
		struct	Token {
			std::string	text;
			int			lineNbr;
		};

		using Iterator	= std::vector<Token>::const_iterator;

	public:
		TokenStream() = delete;
		// TokenStream() = default;
		TokenStream(TokenStream const &) = delete;
		TokenStream(TokenStream &&) = delete;
		TokenStream(std::string const &filePath);
		// TokenStream(std::istream& stream, int streamType);
		~TokenStream() = default;

	private:
		std::vector<Token>	_tokens;
		Iterator			_current;

	public:
		Iterator	current() const;
		Iterator	begin() const;
		Iterator	end() const;

		Token const	&peek() const;
		std::string	consume();
		void		advance();
		bool		atEnd() const;

		Iterator	lineStart() const;
		Iterator	lineEnd() const;
		std::string	getLine() const;
		void		advanceLine();
		void		advanceUntil(const std::string& target);
		std::size_t	tokensOnLine();
};

#endif
