#include "Config.hpp"

std::ostream	&operator<<(std::ostream &os, Config const &config)
{
	if (config.listeners.empty())
		return (os);

	os << "==== CONFIG ====\n";
	for (size_t i = 0; i < config.listeners.size(); ++i)
	{
		const Config::Listener& srv = config.listeners[i];
		os << "\n--- Listener " << i << " ---\n";
		os << "name: " << srv.name << "\n";
		os << "host: " << srv.host << "\n";
		os << "root: " << srv.root << "\n";
		os << "port: " << srv.port << "\n";
		os << "clientMaxBodySize: " << srv.clientMaxBodySize << "\n";

		// Error pages
		os << "Error Pages:\n";
		for (std::pair<u_int16_t, std::string> const &errorPage : srv.errorPages)
		{
			os << "  [" << errorPage.first << "] " << errorPage.second << "\n";
		}

		// Locations
		os << "Locations:\n";
		for (std::pair<std::string, Config::Listener::Location> const &location : srv.locations)
		{
			std::string const					&path	= location.first;
			Config::Listener::Location const	&loc	= location.second;

			os << "  --- Location " << path << " ---\n";
			os << "  root: " << loc.root << "\n";
			os << "  clientMaxBodySize: " << loc.clientMaxBodySize << "\n";
			os << "  returnURL: " << loc.returnURL << "\n";
			os << "  redirectStatus: " << loc.redirectStatus << "\n";
			os << "  autoindex: " << (loc.autoindex ? "true" : "false") << "\n";
			os << "  uploadDir: " << loc.uploadDir << "\n";
			os << "  index: " << loc.index << "\n";
			os << "  cgiEXT: " << loc.cgiEXT << "\n";
			os << "  cgiPath: " << loc.cgiPath << "\n";

			os << "  allowedMethods: ";
			for (const auto& m : loc.allowedMethods)
				os << m << " ";
			os << "\n";

			os << "  indexFiles: ";
			for (const auto& f : loc.indexFiles)
				os << f << " ";
			os << "\n";
		}

		os << "\n";
	}
	os << "==== END CONFIG ====\n";

	return (os);
}
