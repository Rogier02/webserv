#include "../include/Server.hpp"
#include "../include/Config.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    std::string configFile = "./config/webserv.conf";
    
    if (argc > 1) {
        configFile = argv[1];
    }
    
    try {
        Config config(configFile);
        Server server(&config);
        
        std::cout << "Starting webserver..." << std::endl;
        server.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
