# webserv
42_webserv project - Complete Learning Guide

## Table of Contents
1. [Project Overview](#project-overview)
2. [Learning Path](#learning-path)
3. [Step-by-Step Implementation](#step-by-step-implementation)
4. [Resources](#resources)
5. [Testing Strategy](#testing-strategy)

## Project Overview
Building an HTTP/1.1 web server from scratch in C++ that can handle multiple clients, serve files, execute CGI scripts, and parse configuration files.

## Learning Path

### Phase 1: Foundation Knowledge (Week 1-2)
**Goal**: Understand the basics before writing any code

#### 1.1 Networking Fundamentals
**What to learn:**
- How computers communicate over networks
- TCP/IP protocol stack
- Client-server architecture
- Ports and IP addresses

**Study Materials:**
- Read: "Computer Networking: A Top-Down Approach" Chapter 1-2
- Watch: "How does the Internet work?" videos on YouTube
- Practice: Use `telnet` to connect to websites manually

**Hands-on Tasks:**
```bash
# Try these commands to understand networking
telnet google.com 80
nc -l 8080  # Start a simple server
telnet localhost 8080  # Connect to your server
```

#### 1.2 HTTP Protocol Deep Dive
**What to learn:**
- HTTP request/response cycle
- HTTP methods (GET, POST, DELETE)
- HTTP headers and status codes
- URL structure and parsing

**Study Materials:**
- Read: MDN Web Docs on HTTP
- Read: RFC 7230-7235 (HTTP/1.1 specification) - focus on basics
- Use browser developer tools to inspect HTTP traffic

**Hands-on Tasks:**
- Manually craft HTTP requests using telnet
- Analyze HTTP headers in browser dev tools
- Understand difference between HTTP/1.0 and HTTP/1.1

#### 1.3 C++ Systems Programming
**What to learn:**
- Socket programming in C++
- File I/O operations
- String manipulation and parsing
- Error handling and RAII principles

**Study Materials:**
- "Beej's Guide to Network Programming"
- C++ reference documentation on file streams
- Learn about `select()`, `poll()`, or `epoll()` for I/O multiplexing

### Phase 2: Core Implementation (Week 3-8)

## Step-by-Step Implementation

### STEP 1: Project Architecture & Planning (Days 1-3)

#### 1.1 Understanding the Requirements
**Learn about:**
- Software architecture patterns
- Class design principles
- Project structure for large C++ applications

**Action Plan:**
1. Create a basic project structure:
```
webserv/
├── src/
│   ├── server/
│   ├── http/
│   ├── config/
│   └── utils/
├── include/
├── config/
├── www/
├── tests/
└── Makefile
```

2. Design your main classes:
   - `Server` (main server class)
   - `HttpRequest` (parse incoming requests)
   - `HttpResponse` (generate responses)
   - `Config` (configuration parser)
   - `Socket` (socket wrapper)

#### 1.2 Setting Up Development Environment
**Action Items:**
- Set up a proper Makefile
- Create a basic main.cpp
- Set up a simple test website in www/
- Plan your error handling strategy

### STEP 2: Basic Socket Operations (Days 4-10)

#### 2.1 Learn Socket Programming Theory
**Study Topics:**
- TCP sockets vs UDP sockets
- Socket states and lifecycle
- Blocking vs non-blocking I/O
- Address families (IPv4 vs IPv6)

**Essential Functions to Master:**
```cpp
socket()    // Create a socket
bind()      // Bind to an address
listen()    // Listen for connections
accept()    // Accept incoming connections
recv()/send() // Send/receive data
close()     // Close socket
```

#### 2.2 Implement Basic Socket Server
**Learning Goals:**
- Create a socket that listens on a port
- Accept ONE client connection
- Read data from client
- Send a simple response
- Handle basic errors

**Implementation Tasks:**
1. Create a `Socket` class that wraps basic socket operations
2. Implement error handling for each socket operation
3. Test with telnet: `telnet localhost 8080`

**Code Milestone:** A server that accepts one connection and echoes back what it receives.

### STEP 3: HTTP Request Parsing (Days 11-17)

#### 3.1 Learn HTTP Message Format
**Study the structure:**
```
POST /path/to/resource HTTP/1.1
Host: www.example.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 13

param1=value1
```

**Key Components:**
- Request line (method, URI, version)
- Headers (key-value pairs)
- Body (optional, depends on method)

#### 3.2 Implement HTTP Parser
**Learning Goals:**
- Parse HTTP request line
- Parse headers into key-value pairs
- Handle different HTTP methods
- Validate HTTP format
- Handle malformed requests

**Implementation Tasks:**
1. Create `HttpRequest` class
2. Implement line-by-line parsing
3. Handle case-insensitive headers
4. Parse query parameters
5. Validate HTTP version

**Code Milestone:** Parse incoming HTTP requests and print parsed components.

#### 3.3 Handle Different Request Types
**Learn about:**
- GET requests (query parameters)
- POST requests (body parsing)
- DELETE requests
- HEAD requests

### STEP 4: HTTP Response Generation (Days 18-24)

#### 4.1 Learn HTTP Response Format
**Study the structure:**
```
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 88

<html>
<body>Hello World!</body>
</html>
```

#### 4.2 Implement Response Generator
**Learning Goals:**
- Generate proper HTTP response headers
- Set appropriate status codes
- Handle different content types (MIME types)
- Calculate Content-Length correctly

**Implementation Tasks:**
1. Create `HttpResponse` class
2. Implement status code handling
3. Add MIME type detection
4. Generate proper headers
5. Format complete HTTP response

**Code Milestone:** Send proper HTTP responses that browsers can display.

### STEP 5: File Serving System (Days 25-31)

#### 5.1 Learn File System Operations
**Study Topics:**
- File I/O in C++
- Directory traversal
- File permissions and security
- MIME type detection
- Binary vs text files

#### 5.2 Implement Static File Server
**Learning Goals:**
- Serve HTML, CSS, JS, images
- Handle file not found (404)
- Implement directory listing
- Security: prevent directory traversal attacks
- Handle large files efficiently

**Implementation Tasks:**
1. Map URLs to file system paths
2. Implement file reading with proper error handling
3. Add MIME type detection based on file extensions
4. Create directory listing HTML
5. Add security checks (no ../ in paths)

**Code Milestone:** Serve a complete static website with HTML, CSS, and images.

### STEP 6: Multi-Client Handling (Days 32-38)

#### 6.1 Learn I/O Multiplexing
**Study Topics:**
- Problems with blocking I/O
- `select()` system call
- `poll()` vs `epoll()`
- Non-blocking sockets
- Event-driven programming

**Essential Concepts:**
- File descriptor sets
- Timeout handling
- Handling partial reads/writes

#### 6.2 Implement Concurrent Server
**Learning Goals:**
- Handle multiple clients simultaneously
- Use `select()` or equivalent for I/O multiplexing
- Manage client state
- Handle client disconnections gracefully

**Implementation Tasks:**
1. Convert sockets to non-blocking mode
2. Implement `select()` main loop
3. Manage multiple client connections
4. Handle partial HTTP messages
5. Implement proper cleanup on client disconnect

**Code Milestone:** Server can handle 10+ simultaneous browser connections.

### STEP 7: Configuration System (Days 39-45)

#### 7.1 Learn Configuration Patterns
**Study Topics:**
- Configuration file formats
- Nginx/Apache configuration structure
- Virtual hosts concept
- URL routing and location blocks

#### 7.2 Implement Config Parser
**Learning Goals:**
- Parse server blocks (virtual hosts)
- Handle location blocks (URL routing)
- Support multiple server configurations
- Validate configuration syntax

**Implementation Tasks:**
1. Design configuration file format
2. Implement recursive parser
3. Support server blocks for virtual hosts
4. Add location blocks for URL routing
5. Handle error pages and redirects

**Sample Config Structure:**
```
server {
    listen 8080;
    server_name localhost;
    root ./www;
    
    location / {
        methods GET POST;
        index index.html;
    }
    
    location /upload {
        methods POST;
        upload_store ./uploads;
    }
}
```

### STEP 8: CGI Implementation (Days 46-52)

#### 8.1 Learn CGI Protocol
**Study Topics:**
- Common Gateway Interface standard
- Environment variables
- Process creation and management
- Pipe communication
- Security considerations

#### 8.2 Implement CGI Execution
**Learning Goals:**
- Execute external scripts (PHP, Python)
- Set up proper environment variables
- Handle input/output through pipes
- Manage child processes
- Handle timeouts and errors

**Implementation Tasks:**
1. Implement process forking and execution
2. Set up CGI environment variables
3. Handle script input/output
4. Add timeout protection
5. Support common script types

## Resources

### Essential Reading
1. **"Beej's Guide to Network Programming"** - Free online, covers sockets
2. **RFC 7230** - HTTP/1.1 Message Syntax and Routing
3. **"Unix Network Programming" by Stevens** - Advanced networking
4. **MDN Web Docs** - HTTP protocol reference

### Tools for Testing
```bash
# Essential testing tools
curl -v http://localhost:8080/
telnet localhost 8080
nc -l 8080
wireshark  # Network packet analysis
```

### Online Resources
- **HTTP/1.1 Specification**: https://tools.ietf.org/html/rfc7230
- **Socket Programming Tutorial**: Beej's Guide
- **CGI Specification**: https://tools.ietf.org/html/rfc3875

## Testing Strategy

### Phase-by-Phase Testing
1. **Socket Tests**: Use telnet to connect
2. **HTTP Tests**: Use curl with various options
3. **File Serving**: Test with browsers
4. **Concurrent**: Use Apache Bench (`ab`)
5. **CGI**: Test with simple scripts

### Automated Testing
```bash
# Load testing
ab -n 1000 -c 10 http://localhost:8080/

# Functionality testing
curl -X POST -d "data=test" http://localhost:8080/upload
curl -H "Host: example.com" http://localhost:8080/
```

## Implementation Timeline
- **Weeks 1-2**: Learning fundamentals
- **Weeks 3-4**: Basic socket server + HTTP parsing
- **Weeks 5-6**: File serving + response generation
- **Weeks 7-8**: Multi-client + configuration
- **Weeks 9-10**: CGI + testing + refinement

Remember: This is a marathon, not a sprint. Focus on understanding each concept thoroughly before moving to the next step!
