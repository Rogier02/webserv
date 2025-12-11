#ifndef EASYTHROW_HPP
# define EASYTHROW_HPP

// C++
# include <iostream>
# include <stdexcept>
# include <cstring>

namespace detail
{
	inline int	easyThrow(
		std::string const &call,
		int result,
		const char *file,
		int line
	) {
		if (result == -1) throw std::runtime_error(
				static_cast<std::string>("(") + file + ":" + std::to_string(line) + ") "
				+ call + ": " + ((errno) ? std::strerror(errno) : ""));
		return (result);
	}
}

# define EasyThrow(functionCall) \
	::detail::easyThrow(#functionCall, (functionCall), __FILE__, __LINE__)

# define LocatedThrow(message) \
	std::runtime_error(static_cast<std::string>("(") + __FILE__ + ":" + std::to_string(__LINE__) + ") : " + message)

#endif
