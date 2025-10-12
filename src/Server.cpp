#include "../include/Server.hpp"
#include "../include/Config.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

Server::Server(Config* config) : _serverSocket(-1), _config(config) {
    _port = config->getPort();
    _host = config->getHost();
    setupServer();
}

Server::~Server() {
    if (_serverSocket >= 0)
        close(_serverSocket);
    
    for (size_t i = 0; i < _fds.size(); ++i) {
        if (_fds[i].fd >= 0)
            close(_fds[i].fd);
    }
}

void Server::setupServer() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        std::cerr << "Error: Failed to create socket" << std::endl;
        throw std::runtime_error("Socket creation failed");
    }
    
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error: Failed to set socket options" << std::endl;
        close(_serverSocket);
        throw std::runtime_error("Socket option failed");
    }
    
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
    
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(_port);
    
    if (inet_pton(AF_INET, _host.c_str(), &address.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address" << std::endl;
        close(_serverSocket);
        throw std::runtime_error("Invalid address");
    }
    
    if (bind(_serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Error: Failed to bind to port " << _port << std::endl;
        close(_serverSocket);
        throw std::runtime_error("Bind failed");
    }
    
    if (listen(_serverSocket, 10) < 0) {
        std::cerr << "Error: Failed to listen" << std::endl;
        close(_serverSocket);
        throw std::runtime_error("Listen failed");
    }
    
    pollfd serverPollfd;
    serverPollfd.fd = _serverSocket;
    serverPollfd.events = POLLIN;
    serverPollfd.revents = 0;
    _fds.push_back(serverPollfd);
    
    std::cout << "Server listening on " << _host << ":" << _port << std::endl;
}

void Server::run() {
    while (true) {
        int pollCount = poll(&_fds[0], _fds.size(), -1);
        
        if (pollCount < 0) {
            std::cerr << "Error: Poll failed" << std::endl;
            break;
        }
        
        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].revents & POLLIN) {
                if (_fds[i].fd == _serverSocket) {
                    acceptConnection();
                } else {
                    handleClient(_fds[i].fd);
                }
            }
        }
    }
}

void Server::acceptConnection() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        return;
    }
    
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    
    pollfd clientPollfd;
    clientPollfd.fd = clientSocket;
    clientPollfd.events = POLLIN;
    clientPollfd.revents = 0;
    _fds.push_back(clientPollfd);
    
    _clientBuffers[clientSocket] = "";
    
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    std::cout << "New connection from " << clientIP << std::endl;
}

void Server::handleClient(int clientSocket) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead <= 0) {
        close(clientSocket);
        _clientBuffers.erase(clientSocket);
        
        for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) {
            if (it->fd == clientSocket) {
                _fds.erase(it);
                break;
            }
        }
        return;
    }
    
    _clientBuffers[clientSocket] += std::string(buffer, bytesRead);
    
    if (_clientBuffers[clientSocket].find("\r\n\r\n") != std::string::npos ||
        _clientBuffers[clientSocket].find("\n\n") != std::string::npos) {
        
        std::string response = processRequest(_clientBuffers[clientSocket]);
        send(clientSocket, response.c_str(), response.length(), 0);
        
        close(clientSocket);
        _clientBuffers.erase(clientSocket);
        
        for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) {
            if (it->fd == clientSocket) {
                _fds.erase(it);
                break;
            }
        }
    }
}

std::string Server::processRequest(const std::string& rawRequest) {
    Request request(rawRequest);
    
    std::cout << "Request: " << request.getMethod() << " " << request.getUri() << std::endl;
    
    Response response(_config);
    response.buildResponse(request);
    
    return response.toString();
}
