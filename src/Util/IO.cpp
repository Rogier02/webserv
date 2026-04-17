#include "IO.hpp"

namespace IO {
	std::string
	getFileContent(std::string const &filePath)
	{
		std::stringstream	buffer;
		std::ifstream		file(filePath);
		if (!file.is_open())
			return ("");

		buffer << file.rdbuf();
		return (buffer.str());
	}

	::ssize_t
	read(int fd, std::string &dest)
	{
		char		buffer[1024];
		::ssize_t	received = ::read(fd, buffer, sizeof(buffer) - 1);

		if (received > 0)
			dest.append(buffer, received);
		return (received);
	}

	::ssize_t
	write(int fd, std::string &src)
	{
		::ssize_t	written = ::write(fd, src.c_str(), src.length());
		return (written);
	}

	bool
	exists(std::string const &file)
	{
		struct stat	buffer;
		return (stat(file.c_str(), &buffer) == 0);
	}
}
