#include "Config.hpp"

std::ostream	&operator<<(std::ostream &os, Config const &config)
{
	if (config.servers.empty())
		return (os);

	os << "==== CONFIG ====\n";
	for (size_t i = 0; i < config.servers.size(); ++i)
	{
		const Config::Server& srv = config.servers[i];
		os << "\n--- Server " << i << " ---\n";
		os << "name: " << srv.name << "\n";
		os << "host: " << srv.host << "\n";
		os << "root: " << srv.root << "\n";
		os << "port: " << srv.port << "\n";
		os << "clientMaxBodySize: " << srv.clientMaxBodySize << "\n";

		// Error pages
		os << "Error Pages:\n";
		for (size_t e = 0; e < srv.errorPages.size(); ++e)
		{
			const auto& ep = srv.errorPages[e];
			os << "  [" << e << "] "
					<< ep.code << " -> " << ep.path << "\n";
		}

		// Locations
		os << "Locations:\n";
		for (size_t l = 0; l < srv.locations.size(); ++l)
		{
			const Config::Server::Location& loc = srv.locations[l];
			os << "  --- Location " << l << " ---\n";
			os << "  path: " << loc.path << "\n";
			os << "  root: " << loc.root << "\n";
			os << "  clientMaxBodySize: " << loc.clientMaxBodySize << "\n";
			os << "  returnURL: " << loc.returnURL.path << "\n";
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
