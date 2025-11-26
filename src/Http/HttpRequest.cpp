// #include "HttpRequest.hpp"
// #include "TokenStream.hpp"
// #include <vector>
// #include <fstream>
// #include <iostream>

// /*
// -- HTTP1.0 request example --
// GET /index.html HTTP/1.0
// Host: www.example.com
// User-Agent: Mozilla/5.0
// Accept: text/html
// */

// /*
// struct HttpRequest {
//     HttpMethod method;
//     std::string target; // URL path + query
// 	struct	Version{
// 		int httpMajorVersion; // usually 1
// 		int httpMinorVersion; // 0 or 1
// 	};

//     std::map<std::string, std::string> headers;
//     std::string body;

//     // Helper: find header case-insensitively
//     std::string getHeader(const std::string &name) const {
//         for (std::map<std::string, std::string>::const_iterator it = headers.begin();
//              it != headers.end(); ++it)
//         {
//             if (strcasecmp(it->first.c_str(), name.c_str()) == 0)
//                 return it->second;
//         }
//         return "";
//     }
// };
// */

// //1. read first line --> split into method, path, version
// //2. read header lines until empty line
// //3. if content-length exists --> read that many bytes for the body

// void
// HttpRequest::parseHttpRequest(){

// 	std::string request;
// 	std::string line = getline(request);
// 	std::vector<std::string> parts = split(line, ' ');

// 	method = parseMethod(parts[0]);
// 	target = parts[1];
// 	version = paseVersion(parts[2]);


// 	std::vector<Token> httpTokens;
// 	TokenStream httpTS(httpTokens);

// 	httpTS.current
// }

// std::string
// parseMethod(std::string method){
// 	while (allowedMethods){
// 		if (method == allowedMethods)
// 			return (method);
// 	}
// 	else if (method)
// 	else
// 		LOG("[HTTP Request Error] invalid method: " << method << "\n");
// }

// Version
// parseVersion(std::string version){
// 	Version	httpVersion;

// 	std::string::size_type slash = version.find('/');
// 	std::string::size_type dot = version.find('.');

// 	version.httpMajorVersion = std::stoi(version.substr(slash + 1, 1));
// 	version.httpMinorVersion = std::stoi(version.substr(dot + 1, 1));

// 	return (httpVersion);
// }
