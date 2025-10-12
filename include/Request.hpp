#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

class Request {
private:
    std::string _method;
    std::string _uri;
    std::string _httpVersion;
    std::map<std::string, std::string> _headers;
    std::string _body;
    std::string _query;
    
    void parseRequestLine(const std::string& line);
    void parseHeaders(const std::string& headerSection);
    void parseQueryString();
    
public:
    Request(const std::string& rawRequest);
    ~Request();
    
    std::string getMethod() const;
    std::string getUri() const;
    std::string getHttpVersion() const;
    std::string getHeader(const std::string& key) const;
    std::string getBody() const;
    std::string getQuery() const;
    bool isValid() const;
};

#endif
