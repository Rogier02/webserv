#include "../include/Response.hpp"
#include "../include/Request.hpp"
#include "../include/Config.hpp"
#include "../include/Utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

Response::Response(Config* config) : _statusCode(200), _config(config) {
    _statusMessage = "OK";
}

Response::~Response() {}

void Response::setStatus(int code) {
    _statusCode = code;
    _statusMessage = Utils::getStatusMessage(code);
}

void Response::buildResponse(const Request& request) {
    if (!request.isValid()) {
        setStatus(400);
        _body = "<html><body><h1>400 Bad Request</h1></body></html>";
        _headers["Content-Type"] = "text/html";
        return;
    }
    
    std::string method = request.getMethod();
    
    if (!_config->isMethodAllowed(method)) {
        setStatus(405);
        _body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
        _headers["Content-Type"] = "text/html";
        return;
    }
    
    if (method == "GET") {
        handleGetRequest(request);
    } else if (method == "POST") {
        handlePostRequest(request);
    } else if (method == "DELETE") {
        handleDeleteRequest(request);
    } else {
        setStatus(501);
        _body = "<html><body><h1>501 Not Implemented</h1></body></html>";
        _headers["Content-Type"] = "text/html";
    }
    
    _headers["Date"] = Utils::getTimestamp();
    _headers["Server"] = "webserv/1.0";
    _headers["Content-Length"] = Utils::intToString(_body.length());
}

void Response::handleGetRequest(const Request& request) {
    std::string uri = request.getUri();
    std::string path = _config->getRoot() + uri;
    
    if (isDirectory(path)) {
        std::vector<std::string> indexFiles = _config->getIndex();
        bool found = false;
        
        for (size_t i = 0; i < indexFiles.size(); ++i) {
            std::string indexPath = path;
            if (!Utils::endsWith(path, "/"))
                indexPath += "/";
            indexPath += indexFiles[i];
            
            if (fileExists(indexPath)) {
                serveStaticFile(indexPath);
                found = true;
                break;
            }
        }
        
        if (!found) {
            if (_config->getAutoindex()) {
                serveDirectoryListing(path);
            } else {
                setStatus(403);
                _body = "<html><body><h1>403 Forbidden</h1></body></html>";
                _headers["Content-Type"] = "text/html";
            }
        }
    } else if (fileExists(path)) {
        std::string ext = Utils::getExtension(path);
        if (ext == ".py" || ext == ".sh" || ext == ".pl") {
            serveCGI(path, request);
        } else {
            serveStaticFile(path);
        }
    } else {
        setStatus(404);
        _body = "<html><body><h1>404 Not Found</h1></body></html>";
        _headers["Content-Type"] = "text/html";
    }
}

void Response::handlePostRequest(const Request& request) {
    std::string body = request.getBody();
    
    if (body.length() > _config->getClientMaxBodySize()) {
        setStatus(413);
        _body = "<html><body><h1>413 Payload Too Large</h1></body></html>";
        _headers["Content-Type"] = "text/html";
        return;
    }
    
    std::string uri = request.getUri();
    std::string path = _config->getRoot() + uri;
    
    std::string ext = Utils::getExtension(path);
    if (ext == ".py" || ext == ".sh" || ext == ".pl") {
        if (fileExists(path)) {
            serveCGI(path, request);
        } else {
            setStatus(404);
            _body = "<html><body><h1>404 Not Found</h1></body></html>";
            _headers["Content-Type"] = "text/html";
        }
    } else {
        setStatus(201);
        _body = "<html><body><h1>201 Created</h1><p>Resource created successfully</p></body></html>";
        _headers["Content-Type"] = "text/html";
    }
}

void Response::handleDeleteRequest(const Request& request) {
    std::string uri = request.getUri();
    std::string path = _config->getRoot() + uri;
    
    if (!fileExists(path) || isDirectory(path)) {
        setStatus(404);
        _body = "<html><body><h1>404 Not Found</h1></body></html>";
        _headers["Content-Type"] = "text/html";
        return;
    }
    
    if (remove(path.c_str()) == 0) {
        setStatus(204);
        _body = "";
    } else {
        setStatus(500);
        _body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        _headers["Content-Type"] = "text/html";
    }
}

void Response::serveStaticFile(const std::string& path) {
    std::string content = readFile(path);
    if (content.empty() && !fileExists(path)) {
        setStatus(404);
        _body = "<html><body><h1>404 Not Found</h1></body></html>";
        _headers["Content-Type"] = "text/html";
    } else {
        setStatus(200);
        _body = content;
        setContentType(path);
    }
}

void Response::serveCGI(const std::string& path, const Request& request) {
    std::string ext = Utils::getExtension(path);
    std::string interpreter;
    
    if (ext == ".py")
        interpreter = "/usr/bin/python3";
    else if (ext == ".sh")
        interpreter = "/bin/bash";
    else if (ext == ".pl")
        interpreter = "/usr/bin/perl";
    else {
        setStatus(500);
        _body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        _headers["Content-Type"] = "text/html";
        return;
    }
    
    std::string tempInput = "/tmp/cgi_input_" + Utils::intToString(getpid());
    std::string tempOutput = "/tmp/cgi_output_" + Utils::intToString(getpid());
    
    std::ofstream inputFile(tempInput.c_str());
    if (inputFile.is_open()) {
        inputFile << request.getBody();
        inputFile.close();
    }
    
    std::string cmd = interpreter + " " + path + " < " + tempInput + " > " + tempOutput + " 2>&1";
    int result = system(cmd.c_str());
    
    remove(tempInput.c_str());
    
    if (result == 0) {
        _body = readFile(tempOutput);
        setStatus(200);
        
        size_t headerEnd = _body.find("\n\n");
        if (headerEnd != std::string::npos) {
            std::string cgiHeaders = _body.substr(0, headerEnd);
            _body = _body.substr(headerEnd + 2);
            
            std::istringstream stream(cgiHeaders);
            std::string line;
            while (std::getline(stream, line)) {
                line = Utils::trim(line);
                size_t pos = line.find(':');
                if (pos != std::string::npos) {
                    std::string key = Utils::trim(line.substr(0, pos));
                    std::string value = Utils::trim(line.substr(pos + 1));
                    _headers[key] = value;
                }
            }
        } else {
            _headers["Content-Type"] = "text/html";
        }
    } else {
        setStatus(500);
        _body = "<html><body><h1>500 Internal Server Error</h1><p>CGI script failed</p></body></html>";
        _headers["Content-Type"] = "text/html";
    }
    
    remove(tempOutput.c_str());
}

void Response::serveDirectoryListing(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        setStatus(500);
        _body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        _headers["Content-Type"] = "text/html";
        return;
    }
    
    std::ostringstream html;
    html << "<html><head><title>Index of " << path << "</title></head><body>";
    html << "<h1>Index of " << path << "</h1><hr><ul>";
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name == ".")
            continue;
        html << "<li><a href=\"" << name;
        if (entry->d_type == DT_DIR)
            html << "/";
        html << "\">" << name;
        if (entry->d_type == DT_DIR)
            html << "/";
        html << "</a></li>";
    }
    
    html << "</ul><hr></body></html>";
    closedir(dir);
    
    setStatus(200);
    _body = html.str();
    _headers["Content-Type"] = "text/html";
}

bool Response::fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool Response::isDirectory(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0)
        return false;
    return S_ISDIR(buffer.st_mode);
}

std::string Response::readFile(const std::string& path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
        return "";
    
    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    return content.str();
}

void Response::setContentType(const std::string& path) {
    std::string ext = Utils::getExtension(path);
    _headers["Content-Type"] = getContentType(ext);
}

std::string Response::getContentType(const std::string& extension) {
    if (extension == ".html" || extension == ".htm")
        return "text/html";
    if (extension == ".css")
        return "text/css";
    if (extension == ".js")
        return "application/javascript";
    if (extension == ".json")
        return "application/json";
    if (extension == ".png")
        return "image/png";
    if (extension == ".jpg" || extension == ".jpeg")
        return "image/jpeg";
    if (extension == ".gif")
        return "image/gif";
    if (extension == ".svg")
        return "image/svg+xml";
    if (extension == ".txt")
        return "text/plain";
    return "application/octet-stream";
}

std::string Response::toString() const {
    std::ostringstream response;
    response << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
    
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    
    response << "\r\n" << _body;
    return response.str();
}
