#include "../include/Request.hpp"
#include "../include/Utils.hpp"
#include <sstream>

Request::Request(const std::string& rawRequest) {
    if (rawRequest.empty())
        return;
    
    size_t headerEnd = rawRequest.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        headerEnd = rawRequest.find("\n\n");
    
    std::string headerSection;
    if (headerEnd != std::string::npos) {
        headerSection = rawRequest.substr(0, headerEnd);
        _body = rawRequest.substr(headerEnd + 4);
    } else {
        headerSection = rawRequest;
    }
    
    std::istringstream stream(headerSection);
    std::string line;
    
    if (std::getline(stream, line)) {
        parseRequestLine(line);
    }
    
    std::string headers;
    std::string temp;
    while (std::getline(stream, temp)) {
        headers += temp + "\n";
    }
    parseHeaders(headers);
    parseQueryString();
}

Request::~Request() {}

void Request::parseRequestLine(const std::string& line) {
    std::istringstream iss(line);
    iss >> _method >> _uri >> _httpVersion;
    
    _httpVersion = Utils::trim(_httpVersion);
}

void Request::parseHeaders(const std::string& headerSection) {
    std::istringstream stream(headerSection);
    std::string line;
    
    while (std::getline(stream, line)) {
        line = Utils::trim(line);
        if (line.empty())
            continue;
            
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key = Utils::trim(line.substr(0, pos));
            std::string value = Utils::trim(line.substr(pos + 1));
            _headers[key] = value;
        }
    }
}

void Request::parseQueryString() {
    size_t pos = _uri.find('?');
    if (pos != std::string::npos) {
        _query = _uri.substr(pos + 1);
        _uri = _uri.substr(0, pos);
    }
}

std::string Request::getMethod() const {
    return _method;
}

std::string Request::getUri() const {
    return _uri;
}

std::string Request::getHttpVersion() const {
    return _httpVersion;
}

std::string Request::getHeader(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it != _headers.end())
        return it->second;
    return "";
}

std::string Request::getBody() const {
    return _body;
}

std::string Request::getQuery() const {
    return _query;
}

bool Request::isValid() const {
    return !_method.empty() && !_uri.empty() && !_httpVersion.empty();
}
