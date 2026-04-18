#include "Config.hpp"

std::string
Config::toString()
const {
	std::stringstream	stream;

	if (listeners.empty())
		return (stream.str());

	stream << "==== CONFIG ====\n";
	for (::size_t i = 0; i < listeners.size(); ++i)
	{
		const Listener& srv = listeners[i];
		stream << "\n--- Listener " << i << " ---\n";
		stream << "name: " << srv.name << "\n";
		stream << "host: " << srv.host << "\n";
		stream << "root: " << srv.root << "\n";
		stream << "port: " << srv.port << "\n";
		stream << "clientMaxBodySize: " << srv.clientMaxBodySize << "\n";

		// Error pages
		stream << "Error Pages:\n";
		for (std::pair<const u_int16_t, std::string> const &errorPage : srv.errorPages)
		{
			stream << "  [" << errorPage.first << "] " << errorPage.second << "\n";
		}

		// Locations
		stream << "Locations:\n";
		for (std::pair<const std::string, Listener::Location> const &location : srv.locations)
		{
			std::string const					&path	= location.first;
			Listener::Location const	&loc	= location.second;

			stream << "  --- Location " << path << " ---\n";
			stream << "  root: " << loc.root << "\n";
			stream << "  clientMaxBodySize: " << loc.clientMaxBodySize << "\n";
			stream << "  returnURL: " << loc.returnURL << "\n";
			stream << "  redirectStatus: " << loc.redirectStatus << "\n";
			stream << "  autoindex: " << (loc.autoindex ? "true" : "false") << "\n";
			stream << "  uploadDir: " << loc.uploadDir << "\n";
			stream << "  index: " << loc.index << "\n";
			stream << "  cgiEXT: " << loc.cgiEXT << "\n";
			stream << "  cgiPath: " << loc.cgiPath << "\n";
			stream << "  allowedMethods: ";
			for (std::set<std::string>::const_iterator it = loc.allowedMethods.begin();
				it != loc.allowedMethods.end(); ++it)
			{
				if (it != loc.allowedMethods.begin())
					stream << ", ";
				stream << *it;
			}
		}

		stream << "\n";
	}
	stream << "==== END CONFIG ====\n";

	return (stream.str());
}
