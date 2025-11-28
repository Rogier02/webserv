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
# include "MessageException.hpp"

class	Parse
{
	private:
		class UnknownDirective : public MessageException {
			public:
				UnknownDirective(const std::string& directive, int lineNbr, const std::string& line);
		};

	private:
		TokenStream	ts;

	public:
		Parse(std::string const &filePath);
		Parse(Parse const &other) = delete;
		Parse(Parse &&other) = delete;
		~Parse() = default;

	public:
		Config	config();

	private:
		Config::Server				server();
		Config::Server::Location	location();
		Config::Server::Page		page();

		void	single(std::string &dest);
		void	single(int &dest);
		void	multiple(std::vector<std::string> &dest);

		// special directives
		void	page(Config::Server::Page &page);
		void	clientMaxBodySize(size_t &clientMaxBodySize);
		void	listen(std::string &host, int &port);
		void	autoIndex(bool &autoIndex);

	private:
		std::map<std::string, std::function<void (Config::Server &)>>
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

		std::map<std::string, std::function<void (Config::Server::Location &)>>
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

};

#endif
