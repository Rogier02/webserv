#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Request;
class Config;

class Response {
private:
    int _statusCode;
    std::string _statusMessage;
    std::map<std::string, std::string> _headers;
    std::string _body;
    Config* _config;
    
    void setStatus(int code);
    void setContentType(const std::string& path);
    void handleGetRequest(const Request& request);
    void handlePostRequest(const Request& request);
    void handleDeleteRequest(const Request& request);
    void serveStaticFile(const std::string& path);
    void serveCGI(const std::string& path, const Request& request);
    void serveDirectoryListing(const std::string& path);
    bool fileExists(const std::string& path);
    bool isDirectory(const std::string& path);
    std::string getContentType(const std::string& extension);
    std::string readFile(const std::string& path);
    
public:
    Response(Config* config);
    ~Response();
    
    void buildResponse(const Request& request);
    std::string toString() const;
};

#endif
