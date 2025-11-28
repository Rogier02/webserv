#ifndef ERRORPAGEHANDLER_HPP
# define ERRORPAGEHANDLER_HPP

#include <string>
#include <map>

class ErrorPageHandler
{
	public:
		ErrorPageHandler();
		~ErrorPageHandler();

	private:
		static const std::string _errorPageDirectory;
		std::map<int, std::string> _errorPages;

	public:
		std::string getErrorPage(int statusCode);
		void		setErrorPage(int statusCode, const std::string& filePath);
		std::string	getDefaultErrorPage(int statusCode) const;

	private:
		std::string loadErrorPageFromFile(const std::string& filePath) const;
		
};

#endif