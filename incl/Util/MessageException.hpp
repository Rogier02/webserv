#ifndef MESSAGEEXCEPTION_HPP
# define MESSAGEEXCEPTION_HPP

// C++
# include <string>
# include <exception>

class	MessageException : std::exception
{
	public:
		MessageException(std::string const &message = "No apparent reason...");
		virtual	~MessageException() = default;

	protected:
		std::string	_message;

	public:
		const char	*what() const throw();
};

#endif
