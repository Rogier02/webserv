#ifndef EASYPRINT_HPP
# define EASYPRINT_HPP

// C++
# include <iostream>
# include <cstring>

namespace detail
{
	template <typename T>
	concept Printable = requires(std::ostream &os, T const &t) {
		os << t;
	};

	template <typename T>
	concept Convertable = requires(T const &t) {
		std::to_string(t);
	};

	template <typename T>
	inline T	easyPrint(
		std::string const &call,
		T result,
		const char *file,
		int line
	) {

		if constexpr (Printable<T>)
			std::cout	<< "(" << file << ":" << std::to_string(line) << ") "
						<< call << ": " << result << "\n";
		else if constexpr (Convertable<T>)
			std::cout	<< "(" << file << ":" << std::to_string(line) << ") "
						<< call << ": " << std::to_string(result) << "\n";
		return (result);
	}
}

# define EasyPrint(functionCall) ::detail::easyPrint(#functionCall, (functionCall), __FILE__, __LINE__)

#endif
