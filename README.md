# webserv
42_webserv project - HTTP/1.1 Web Server in C++

## Description
A fully functional HTTP/1.1 web server built from scratch in C++98. This project implements a non-blocking, event-driven server capable of handling multiple clients simultaneously using `poll()`.

## Features
- ✅ HTTP/1.1 compliant request/response handling
- ✅ Non-blocking I/O with poll()
- ✅ Support for GET, POST, and DELETE methods
- ✅ Static file serving with proper MIME types
- ✅ CGI support (Python, Bash, Perl)
- ✅ Directory listing (autoindex)
- ✅ Configuration file support
- ✅ Error handling and HTTP status codes
- ✅ Request body size limiting
- ✅ Multiple clients handling

## Requirements
- C++ compiler with C++98 support (g++, clang++)
- Make
- Python 3 (for CGI scripts)
- Unix-like operating system (Linux, macOS)

## Building
```bash
make
```

This will compile the server and create the `webserv` executable.

## Running
```bash
./webserv [config_file]
```

If no config file is specified, it will use `./config/webserv.conf` by default.

Example:
```bash
./webserv
```

The server will start and listen on the configured port (default: 8080).

## Configuration
Edit `config/webserv.conf` to customize server settings:

```
port=8080                    # Server port
host=127.0.0.1              # Server host
server_name=webserv         # Server name
root=./www                  # Document root
client_max_body_size=1048576 # Max body size (bytes)
autoindex=on                # Enable directory listing
upload_path=./www/uploads   # Upload directory
```

## Testing
Once the server is running, you can test it with:

### Using a web browser
Open your browser and navigate to:
- http://localhost:8080/
- http://localhost:8080/test.html
- http://localhost:8080/cgi/hello.py

### Using curl
```bash
# Test GET request
curl http://localhost:8080/

# Test static file
curl http://localhost:8080/test.html

# Test Python CGI
curl http://localhost:8080/cgi/hello.py

# Test POST request with data
curl -X POST -d "test data" http://localhost:8080/cgi/hello.py

# Test Bash CGI
curl http://localhost:8080/cgi/hello.sh

# Test directory listing
curl http://localhost:8080/cgi/

# Test DELETE method (be careful!)
curl -X DELETE http://localhost:8080/test.txt
```

## Project Structure
```
webserv/
├── Makefile              # Build configuration
├── config/
│   └── webserv.conf     # Server configuration
├── include/             # Header files
│   ├── Server.hpp
│   ├── Config.hpp
│   ├── Request.hpp
│   ├── Response.hpp
│   └── Utils.hpp
├── src/                 # Source files
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Config.cpp
│   ├── Request.cpp
│   ├── Response.cpp
│   └── Utils.cpp
└── www/                 # Web root directory
    ├── index.html
    ├── test.html
    ├── upload.html
    └── cgi/            # CGI scripts
        ├── hello.py
        ├── hello.sh
        └── upload.py
```

## Implementation Details

### Server Architecture
- Uses `poll()` for non-blocking I/O multiplexing
- Single-threaded event loop
- Socket programming with proper error handling
- Graceful connection handling

### HTTP Parser
- Complete HTTP/1.1 request parsing
- Header parsing
- Query string support
- POST body handling

### Response Generation
- Proper HTTP status codes
- Content-Type detection
- Custom error pages
- CGI output parsing

### CGI Support
- Environment variable setup
- Standard input/output redirection
- Support for Python, Bash, and Perl scripts
- Proper CGI header parsing

## Cleaning Up
```bash
# Remove object files
make clean

# Remove object files and executable
make fclean

# Rebuild from scratch
make re
```

## Author
42 School Project

## License
This project is part of the 42 school curriculum.
