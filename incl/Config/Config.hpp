#ifndef CONFIG_HPP
# define CONFIG_HPP

// C++
# include <string>
# include <vector>
// webserb
# include "TokenStream.hpp"
# include "log.hpp"

class	Config
{
	public:
		struct	Server {
			struct Page {
				int			code = 0;
				std::string	path;
			};

			struct ErrorPage : Page {
				ErrorPage(TokenStream &tokens);
			};

			struct Location	{
				std::string	path;
				std::string	root;
				size_t		clientMaxBodySize = 0;
				Page		returnURL;
				int			redirectStatus = 0;
				bool		autoindex = false;
				std::string	uploadDir;
				std::string	index;
				std::string	cgiEXT;
				std::string	cgiPath;

				std::vector<std::string>	allowedMethods;
				std::vector<std::string>	indexFiles;
				// Location(TokenStream &tokens);
			};

			std::string	name;
			std::string	host;
			std::string	root;
			int			port;
			size_t		clientMaxBodySize;

			std::vector<ErrorPage>	errorPages;
			std::vector<Location>	locations;
			Server(TokenStream &tokens);
		};

	public:
		Config() = delete;
		Config(std::string const &filePath);
		Config(Config const &other) = delete;
		Config(Config &&other) = delete;
		~Config() = default;

	private:
		class Error: public std::exception {
			public:
				Error(std::string const &message = "No apparent reason...");
				const char	*what() const throw();
			private:
				std::string	_message;
		};

	public:
		std::vector<Server>	servers;

	private:
		TokenStream	_tokens;

	public:
		void	print();
};

#endif
