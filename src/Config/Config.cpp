#include "Config.hpp"

// temporary
Config::Server::Location	location(TokenStream &ts);
size_t						parseClientMaxBodySize(TokenStream &ts);

Config::Config(std::string const &filePath)
	:	_tokens(filePath)
{
	while (!_tokens.atEnd() && _tokens.current().text != "}")
	{
		if (_tokens.takeToken() == "Server")
			servers.push_back(Server(_tokens));
	}
}

Config::Server::Server(TokenStream &tokens)
{
	tokens.expect("{");
	while (!tokens.atEnd() && tokens.current().text != "}")
	{
		std::string current = tokens.takeToken();
		if (current == "server_name") {
			name = tokens.takeToken();
			tokens.checkSemicolon();
		}
		else if (current == "listen") {
			std::string	hostPort = tokens.takeToken();
			size_t colonPos = hostPort.find(':');
			if (colonPos == std::string::npos)
				LOG("listen didn't have a colon, bad bad");
			host = hostPort.substr(0, colonPos);
			port = std::stoi(hostPort.substr(colonPos + 1));
			tokens.checkSemicolon();
		}
		else if (current == "client_max_body_size")
			clientMaxBodySize = parseClientMaxBodySize(tokens);
		else if (current == "root") {
			root = tokens.takeToken();
			tokens.checkSemicolon();
		}
		else if (current == "error_page")
			errorPages.push_back(ErrorPage(tokens));
		else if (current == "location")
			locations.push_back(location(tokens));
		else {
			LOG("[Config Error] \t\"" << tokens.current().text << "\" -> Unknown directive on line " << tokens.current().lineNbr << ": " << tokens.getLine());
			tokens.setIndex(tokens.lastTokenOnLine() + 1); //check if this works correctly
		}
	}
	tokens.expect("}");
}

Config::Server::ErrorPage::ErrorPage(TokenStream &tokens)
{
	code = std::stoi(tokens.takeToken());
	path = tokens.takeToken();
	tokens.checkSemicolon();
}

Config::Error::Error(std::string const &message)
	:	_message(message)
{}

const char	*Config::Error::what() const throw() {
	return (_message.c_str());
}

// Config::print van chatGPT
void
Config::print()
{
    std::cout << "==== CONFIG ====\n";

    for (size_t i = 0; i < servers.size(); ++i)
    {
        const Config::Server& srv = servers[i];
        std::cout << "\n--- Server " << i << " ---\n";
        std::cout << "name: " << srv.name << "\n";
        std::cout << "host: " << srv.host << "\n";
        std::cout << "root: " << srv.root << "\n";
        std::cout << "port: " << srv.port << "\n";
        std::cout << "clientMaxBodySize: " << srv.clientMaxBodySize << "\n";

        // Error pages
        std::cout << "Error Pages:\n";
        for (size_t e = 0; e < srv.errorPages.size(); ++e)
        {
            const auto& ep = srv.errorPages[e];
            std::cout << "  [" << e << "] "
                      << ep.code << " -> " << ep.path << "\n";
        }

        // Locations
        std::cout << "Locations:\n";
        for (size_t l = 0; l < srv.locations.size(); ++l)
        {
            const Config::Server::Location& loc = srv.locations[l];
            std::cout << "  --- Location " << l << " ---\n";
            std::cout << "  path: " << loc.path << "\n";
            std::cout << "  root: " << loc.root << "\n";
            std::cout << "  clientMaxBodySize: " << loc.clientMaxBodySize << "\n";
            std::cout << "  returnURL: " << loc.returnURL.path << "\n";
            std::cout << "  redirectStatus: " << loc.redirectStatus << "\n";
            std::cout << "  autoindex: " << (loc.autoindex ? "true" : "false") << "\n";
            std::cout << "  uploadDir: " << loc.uploadDir << "\n";
            std::cout << "  index: " << loc.index << "\n";
            std::cout << "  cgiEXT: " << loc.cgiEXT << "\n";
            std::cout << "  cgiPath: " << loc.cgiPath << "\n";

            std::cout << "  allowedMethods: ";
            for (const auto& m : loc.allowedMethods)
                std::cout << m << " ";
            std::cout << "\n";

            std::cout << "  indexFiles: ";
            for (const auto& f : loc.indexFiles)
                std::cout << f << " ";
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    std::cout << "==== END CONFIG ====\n";
}