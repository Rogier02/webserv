#include "Config.hpp"
#include "log.hpp"
#include "TokenStream.hpp"
#include "parsing.hpp"
#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Config::Config(){}

// Config &
// Config::operator=(Config &&other)
// {
// 	if (this != &other)
// 	{
// 		this = other;
// 	}
// 	return (*this);
// }

// Config::Error::Error(std::string const &message)
// 	:	_message(message)
// {}

// const char *
// Config::Error::what()
// const throw() {
// 	return (_message.c_str());
// }

void
Config::getFileContent(std::string fileName){
	if (fileName.size() >= 5 && fileName.rfind(".conf") != fileName.size() - 5)
		std::cout << "Error: incorrect file extension\n"; //return, error, exit?
	std::ifstream file(fileName);
	if (!file.is_open()){
		std::cout << "Error: couldn't open file\n"; //return, error, exit?
	}
	std::string line = "";
	std::vector<Token> tokens;
	int lineNbr = 0;
	while (std::getline(file, line)){
		lineNbr++;

		std::stringstream ss(line);
		std::string word;

		while (ss >> word){
			if (word[0] == '#')
				break;
			if (!word.empty() && word.back() == ';') {
				word.pop_back();
				if (!word.empty()) 
					tokens.push_back({word, lineNbr});
				tokens.push_back({";", lineNbr});
			}
			else
				tokens.push_back({word, lineNbr});
		}
	}
	file.close();
	TokenStream ts(tokens);
	// ts.printTokens();
	loadFromFile(ts);
	// printConfig(*this);
}

void
Config::loadFromFile(TokenStream &ts){
	while (!ts.atEnd() && ts.current().text != "}"){
		if (ts.takeToken() == "Server")
			servers.push_back(parseServer(ts));
	}
}

//printConfig van chatGPT
void
Config::printConfig(const Config& cfg)
{
    std::cout << "==== CONFIG ====\n";

    for (size_t i = 0; i < cfg.servers.size(); ++i)
    {
        const Config::Server& srv = cfg.servers[i];
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