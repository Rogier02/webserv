#include "IO.hpp"

namespace IO {
	std::string
	getFileContent(std::string const &filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open()) {
			return ("");
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return (buffer.str());
	}

	::ssize_t
	read(int fd, std::string &dest)
	{
		char	buffer[1024];

		::ssize_t	received = ::read(fd, buffer, sizeof(buffer) - 1);

		if (received > 0)
			dest.append(buffer, received);

		return (received);
	}

	bool
	exists(std::string const &file)
	{
		struct stat	buffer;
		return (stat(file.c_str(), &buffer) == 0);
	}
}
