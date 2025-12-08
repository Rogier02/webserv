#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// C
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
// C++
# include <string>
# include <vector>
# include <fstream>
# include <iostream>


struct	HttpRequest
{
	struct Header {
		std::string	headerName;
		std::string	headerValue;
	};

	struct HttpVersion {
		int	major;
		int	minor;
	};
	
	int					method;
	std::string			target;
	std::string			path;
	std::string			query;
	HttpVersion			version;
	std::vector<Header>	headers;
};

std::ostream	&operator<<(std::ostream &os, HttpRequest const &httpRequest);

#endif
