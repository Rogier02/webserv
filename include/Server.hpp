#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

class Config;
class Request;
class Response;

class Server {
private:
    int _serverSocket;
    int _port;
    std::string _host;
    std::vector<pollfd> _fds;
    std::map<int, std::string> _clientBuffers;
    Config* _config;
    
    void setupServer();
    void acceptConnection();
    void handleClient(int clientSocket);
    std::string processRequest(const std::string& rawRequest);
    
public:
    Server(Config* config);
    ~Server();
    
    void run();
};

#endif
