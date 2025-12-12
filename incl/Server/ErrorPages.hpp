#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include <string>
# include <map>

# include <fstream>
# include <sstream>

class ErrorPages
{
	private:
		struct Page {
			int			code;
			std::string	title;
			std::string	path;
			std::string	content;
		};

	public:
		ErrorPages();
		~ErrorPages();

	private:
		static const std::string	_directory;
		static std::map<int, Page>	_pages;

	public:
		static std::string getErrorPage(int statusCode);

	private:
		static Page const	&getPage(int code);
};

#endif