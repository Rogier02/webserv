#ifndef IO_HPP
# define IO_HPP

// C
# include <sys/stat.h>
# include <unistd.h>
// C++
# include <string>
# include <fstream>
# include <sstream>

namespace IO {
	std::string	getFileContent(std::string const &path);
	::ssize_t	read(int fd, std::string &dest);
	bool		exists(std::string const &file);
}

#endif
