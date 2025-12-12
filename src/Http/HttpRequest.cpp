#include "HttpRequest.hpp"
#include "TokenStream.hpp"

/*
-- HTTP1.0 request example --
GET /index.html HTTP/1.0\r\n
Host: www.example.com\r\n
User-Agent: Mozilla/5.0\r\n
Accept: text/html\r\n
\r\n
*/

// Even hier geparkeerd:

	// int			clientFd;
	// char		buffer[4096];
	// std::string	request;
	// std::string line;
	// while (true)
	// {
	// 	ssize_t	n	=	recv(clientFd, buffer, 4096, 0);
	// 	request.append(buffer);
	// 	if (request.find("\r\n\r\n") != std::string::npos)
	// 		break;
	// }

	// ParseHttpRequest	parser(request, HTTPREQUEST, config);
	// HttpRequest		httpRequest = parser.httpRequest();
	// std::cout << httpRequest;









// Adjust this to the HTTPREQUEST class

// std::ostream	&operator<<(std::ostream &os, HttpRequest const &httpRequest)
// {
// 	if (httpRequest.servers.empty())
// 		return (os);

// 	os << "==== HttpRequest ====\n";
// 	for (size_t i = 0; i < httpRequest.servers.size(); ++i)
// 	{
// 		const httpRequest::Server& srv = httpRequest.servers[i];
// 		os << "\n--- Server " << i << " ---\n";
// 		os << "name: " << srv.name << "\n";
// 		os << "host: " << srv.host << "\n";
// 		os << "root: " << srv.root << "\n";
// 		os << "port: " << srv.port << "\n";
// 		os << "clientMaxBodySize: " << srv.clientMaxBodySize << "\n";

// 		// Error pages
// 		os << "Error Pages:\n";
// 		for (size_t e = 0; e < srv.errorPages.size(); ++e)
// 		{
// 			const auto& ep = srv.errorPages[e];
// 			os << "  [" << e << "] "
// 					<< ep.code << " -> " << ep.path << "\n";
// 		}

// 		// Locations
// 		os << "Locations:\n";
// 		for (size_t l = 0; l < srv.locations.size(); ++l)
// 		{
// 			const HttpRequest::Server::Location& loc = srv.locations[l];
// 			os << "  --- Location " << l << " ---\n";
// 			os << "  path: " << loc.path << "\n";
// 			os << "  root: " << loc.root << "\n";
// 			os << "  clientMaxBodySize: " << loc.clientMaxBodySize << "\n";
// 			os << "  returnURL: " << loc.returnURL.path << "\n";
// 			os << "  redirectStatus: " << loc.redirectStatus << "\n";
// 			os << "  autoindex: " << (loc.autoindex ? "true" : "false") << "\n";
// 			os << "  uploadDir: " << loc.uploadDir << "\n";
// 			os << "  index: " << loc.index << "\n";
// 			os << "  cgiEXT: " << loc.cgiEXT << "\n";
// 			os << "  cgiPath: " << loc.cgiPath << "\n";

// 			os << "  allowedMethods: ";
// 			for (const auto& m : loc.allowedMethods)
// 				os << m << " ";
// 			os << "\n";

// 			os << "  indexFiles: ";
// 			for (const auto& f : loc.indexFiles)
// 				os << f << " ";
// 			os << "\n";
// 		}

// 		os << "\n";
// 	}
// 	os << "==== END HTTPREQUEST ====\n";

// 	return (os);
// }
