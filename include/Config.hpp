#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <vector>

struct ServerConfig {
    int port;
    std::string host;
    std::string serverName;
    std::string root;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize;
    std::map<std::string, std::string> locations;
    std::vector<std::string> index;
    bool autoindex;
    std::string uploadPath;
    std::map<std::string, bool> allowedMethods;
};

class Config {
private:
    ServerConfig _serverConfig;
    std::string _configFile;
    
    void parseConfigFile();
    void setDefaults();
    std::string trim(const std::string& str);
    
public:
    Config(const std::string& configFile);
    ~Config();
    
    const ServerConfig& getServerConfig() const;
    int getPort() const;
    std::string getHost() const;
    std::string getRoot() const;
    size_t getClientMaxBodySize() const;
    std::vector<std::string> getIndex() const;
    bool getAutoindex() const;
    std::string getUploadPath() const;
    bool isMethodAllowed(const std::string& method) const;
};

#endif
