#include "MessageException.hpp"

MessageException::MessageException(std::string const &message)
	:	_message(message)
{}

const char	*
MessageException::what()
const throw() {
	return (_message.c_str());
}
