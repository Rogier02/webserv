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
		Parse(Parse const &) = delete;
		Parse(Parse &&) = delete;
		Parse(std::string const &filePath);
		~Parse() = default;

	public:
		Config	config();

	private:
		Config::Listener			server();
		Config::Listener::Location	location(std::string const &root);
		Config::Listener::Page		page();

		// get simple value(s)
		void	single(std::string &dest);
		void	single(int &dest);
		void	multiple(std::vector<std::string> &dest);

		// get complex directive value(s)
		void	page(Config::Listener::Page &page);
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
		std::string	unexpectedTokenCount(std::string expected, size_t found);

	private:
		using	ServerDirective = std::function<void (Config::Listener &)>;
		std::map<std::string, ServerDirective>
		serverDirectives = {
			{"server_name",
				[this](Config::Listener& s)
				{ single(s.name); }},
			{"listen",
				[this](Config::Listener& s)
				{ listen(s.host, s.port); }},
			{"client_max_body_size",
				[this](Config::Listener& s)
				{ clientMaxBodySize(s.clientMaxBodySize); }},
			{"root",
				[this](Config::Listener& s)
				{ single(s.root); }},
			{"error_page",
				[this](Config::Listener& s)
				{ s.errorPages.push_back(page()); }},
			{"location",
				[this](Config::Listener& s)
				{ s.locations.push_back(location(s.root)); }},
		};

		using	LocationDirective = std::function<void (Config::Listener::Location &)>;
		std::map<std::string, LocationDirective>
		locationDirectives = {
			{"root",
				[this](Config::Listener::Location& l)
				{ single(l.root); }},
			{"client_max_body_size",
				[this](Config::Listener::Location& l)
				{ clientMaxBodySize(l.clientMaxBodySize); }},
			{"return",
				[this](Config::Listener::Location& l)
				{ page(l.returnURL); }},
			{"redirectStatus",
				[this](Config::Listener::Location& l)
				{ single(l.redirectStatus); }},
			{"autoindex",
				[this](Config::Listener::Location& l)
				{ autoIndex(l.autoindex); }},
			{"upload_dir",
				[this](Config::Listener::Location& l)
				{ single(l.uploadDir); }},
			{"index",
				[this](Config::Listener::Location& l)
				{ single(l.index); }},
			{"cgi_ext",
				[this](Config::Listener::Location& l)
				{ single(l.cgiEXT); }},
			{"cgi_path",
				[this](Config::Listener::Location& l)
				{ single(l.cgiPath); }},
			{"allowed_methods",
				[this](Config::Listener::Location& l)
				{ multiple(l.allowedMethods); }},
			{"index_files",
				[this](Config::Listener::Location& l)
				{ multiple(l.indexFiles); }},
		};
		template <typename D>
		using	DirectiveMapIterator = typename std::map<std::string, D>::iterator;
};

#endif
