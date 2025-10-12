#include "../include/Config.hpp"
#include "../include/Utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Config::Config(const std::string& configFile) : _configFile(configFile) {
    setDefaults();
    parseConfigFile();
}

Config::~Config() {}

void Config::setDefaults() {
    _serverConfig.port = 8080;
    _serverConfig.host = "127.0.0.1";
    _serverConfig.serverName = "webserv";
    _serverConfig.root = "./www";
    _serverConfig.clientMaxBodySize = 1048576; // 1MB
    _serverConfig.autoindex = false;
    _serverConfig.uploadPath = "./www/uploads";
    
    _serverConfig.index.push_back("index.html");
    _serverConfig.index.push_back("index.htm");
    
    _serverConfig.allowedMethods["GET"] = true;
    _serverConfig.allowedMethods["POST"] = true;
    _serverConfig.allowedMethods["DELETE"] = true;
}

std::string Config::trim(const std::string& str) {
    return Utils::trim(str);
}

void Config::parseConfigFile() {
    std::ifstream file(_configFile.c_str());
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file '" << _configFile 
                  << "'. Using defaults." << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue;
            
        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;
            
        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));
        
        if (key == "port") {
            std::istringstream(value) >> _serverConfig.port;
        } else if (key == "host") {
            _serverConfig.host = value;
        } else if (key == "server_name") {
            _serverConfig.serverName = value;
        } else if (key == "root") {
            _serverConfig.root = value;
        } else if (key == "client_max_body_size") {
            std::istringstream(value) >> _serverConfig.clientMaxBodySize;
        } else if (key == "autoindex") {
            _serverConfig.autoindex = (value == "on" || value == "true");
        } else if (key == "upload_path") {
            _serverConfig.uploadPath = value;
        }
    }
    
    file.close();
}

const ServerConfig& Config::getServerConfig() const {
    return _serverConfig;
}

int Config::getPort() const {
    return _serverConfig.port;
}

std::string Config::getHost() const {
    return _serverConfig.host;
}

std::string Config::getRoot() const {
    return _serverConfig.root;
}

size_t Config::getClientMaxBodySize() const {
    return _serverConfig.clientMaxBodySize;
}

std::vector<std::string> Config::getIndex() const {
    return _serverConfig.index;
}

bool Config::getAutoindex() const {
    return _serverConfig.autoindex;
}

std::string Config::getUploadPath() const {
    return _serverConfig.uploadPath;
}

bool Config::isMethodAllowed(const std::string& method) const {
    std::map<std::string, bool>::const_iterator it = _serverConfig.allowedMethods.find(method);
    if (it != _serverConfig.allowedMethods.end())
        return it->second;
    return false;
}
