#ifndef PARSE_HPP
# define PARSE_HPP

// C++
# include <string>
# include <vector>
# include <functional>
# include <map>
// webserv
# include "Config.hpp"
# include "TokenStream.hpp"
# include "Logger.hpp"

class	Parse
{
	private:
		std::vector<std::string>	_log;
		TokenStream					_ts;

	public:
		Parse() = delete;
		Parse(Parse const &other) = delete;
		Parse(Parse &&other) = delete;
		Parse(std::string const &filePath);
		~Parse() = default;

	public:
		Config	config();

	private:
		Config::Server				server();
		Config::Server::Location	location();
		Config::Server::Page		page();

		// get simple value(s)
		void	single(std::string &dest);
		void	single(int &dest);
		void	multiple(std::vector<std::string> &dest);

		// get complex directive value(s)
		void	page(Config::Server::Page &page);
		void	clientMaxBodySize(size_t &clientMaxBodySize);
		void	listen(std::string &host, int &port);
		void	autoIndex(bool &autoIndex);

		// tools
		void	expect(std::string const &expected);

		// keep an error log
		void	log(std::string const &message);
		void	report();

		// error format
		std::string	unknownDirective(const std::string& directive);
		std::string	unexpected(std::string const &expected, std::string const &found);
		std::string	unexpectedTokenCount(size_t expected, size_t found);

	private:
		using	ServerDirective = std::function<void (Config::Server &)>;
		std::map<std::string, ServerDirective>
		serverDirectives = {
			{"server_name",
				[this](Config::Server& s)
				{ single(s.name); }},
			{"listen",
				[this](Config::Server& s)
				{ listen(s.host, s.port); }},
			{"client_max_body_size",
				[this](Config::Server& s)
				{ clientMaxBodySize(s.clientMaxBodySize); }},
			{"root",
				[this](Config::Server& s)
				{ single(s.root); }},
			{"error_page",
				[this](Config::Server& s)
				{ s.errorPages.push_back(page()); }},
			{"location",
				[this](Config::Server& s)
				{ s.locations.push_back(location()); }},
		};

		using	LocationDirective = std::function<void (Config::Server::Location &)>;
		std::map<std::string, LocationDirective>
		locationDirectives = {
			{"root",
				[this](Config::Server::Location& l)
				{ single(l.root); }},
			{"client_max_body_size",
				[this](Config::Server::Location& l)
				{ clientMaxBodySize(l.clientMaxBodySize); }},
			{"return",
				[this](Config::Server::Location& l)
				{ page(l.returnURL); }},
			{"redirectStatus",
				[this](Config::Server::Location& l)
				{ single(l.redirectStatus); }},
			{"autoindex",
				[this](Config::Server::Location& l)
				{ autoIndex(l.autoindex); }},
			{"upload_dir",
				[this](Config::Server::Location& l)
				{ single(l.uploadDir); }},
			{"index",
				[this](Config::Server::Location& l)
				{ single(l.index); }},
			{"cgi_ext",
				[this](Config::Server::Location& l)
				{ single(l.cgiEXT); }},
			{"cgi_path",
				[this](Config::Server::Location& l)
				{ single(l.cgiPath); }},
			{"allowed_methods",
				[this](Config::Server::Location& l)
				{ multiple(l.allowedMethods); }},
			{"index_files",
				[this](Config::Server::Location& l)
				{ multiple(l.indexFiles); }},
		};
		template <typename D>
		using	DirectiveMapIterator = typename std::map<std::string, D>::iterator;
};

#endif
