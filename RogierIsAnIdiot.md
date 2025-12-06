# Webserv Architecture Guide

## Overview

Webserv is an event-driven HTTP/1.1 server built on Linux epoll. This document explains the major components and how they interact.

```
┌─────────────────────────────────────────────────────────────┐
│                        main.cpp                             │
│                    (Entry Point)                            │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
        ┌────────────────────────────────┐
        │      Server::run()             │
        │   (Main Event Loop)            │
        │                                │
        │  while (_running) {            │
        │    events = epoll.wait()       │
        │    for (event in events) {     │
        │      handleEvent(event)        │
        │    }                           │
        │  }                             │
        └────────────┬───────────────────┘
                     │
         ┌───────────┴───────────┐
         ▼                       ▼
    ┌─────────────┐      ┌─────────────────┐
    │ New Client  │      │ Existing Client │
    │ (newClient) │      │(existingClient) │
    │             │      │                 │
    │ accept()    │      │ recv()          │
    │ add to epoll│      │ parse request   │
    └─────────────┘      │ generate resp   │
                         │ send()          │
                         └─────────────────┘
```

## Component Breakdown

### 1. **Epoll System** (The Event Watcher)
**Files**: `incl/Epoll.hpp`, `src/Server/Epoll.cpp`

**Purpose**: Monitor multiple file descriptors for I/O readiness

**Key Concept**: Instead of checking each socket individually (wasteful), epoll tells us "these sockets have activity NOW"

**Workflow**:
```
1. Create epoll instance
2. Add listening socket to epoll
   epoll.ctl(Add, listening_socket_fd)
3. Enter main loop
4. epoll.wait() blocks here (0% CPU)
5. Client connects or sends data
6. epoll.wait() returns immediately
7. Process events, then loop back to step 4
```

**Key Methods**:
- `epoll.wait()` - Block until activity, return list of active events
- `epoll.ctl(Add, fd)` - Start monitoring a file descriptor
- `epoll.ctl(Del, fd)` - Stop monitoring a file descriptor

**Why Epoll?**
- `select()`: Check max 1024 sockets, O(n) complexity
- `poll()`: Check unlimited sockets, but still O(n)
- `epoll`: Unlimited sockets, O(1) complexity, only returns active ones

### 2. **Event System** (The Event Object)
**Files**: `incl/Event.hpp`, `src/Server/Event.cpp`

**Purpose**: Represent an I/O event that occurred

**What is an Event?**
When epoll tells us "something happened", it gives us an event object that contains:
- `data.fd` - Which file descriptor had activity
- `events` - What kind of activity (read ready, write ready, error, hangup)

**Key Methods**:
- `event.fd()` - Get the file descriptor that had activity
- `event.isWeird()` - Check if error or unexpected event occurred

**Example**:
```cpp
std::vector<Epoll::Event> events = epoll.wait();
for (auto &event : events) {
    int fd = event.fd();
    
    if (event.isWeird()) {
        // Error or connection closed
        closeClient(fd);
    } else if (fd == listening_socket) {
        // New connection incoming
        newClient();
    } else {
        // Existing client sent data
        existingClient(fd);
    }
}
```

### 3. **Server** (The Orchestrator)
**Files**: `incl/Server.hpp`, `src/Server/Server.cpp`

**Purpose**: Main server logic and event routing

**Responsibilities**:
1. Create listening socket (bound to port 8080)
2. Create epoll instance to monitor it
3. Run main event loop
4. Route events to appropriate handlers
5. Manage client connections

**State Management**:
Currently, the server is stateless. All client data (partial requests, etc.) must be stored somewhere. This is a TODO for later.

### 4. **Socket System** (Network I/O Wrappers)
**Files**: 
- `incl/Socket.hpp`, `src/Sockets/Socket.cpp`
- `incl/ListenSocket.hpp`, `src/Sockets/ListenSocket.cpp`

**Purpose**: C++ wrappers around raw socket functions

**Layers**:
```
┌──────────────────────────────────────┐
│ Your Code (Server, Client handlers)  │
└────────────────┬─────────────────────┘
                 │
         ┌───────▼────────┐
         │ ListenSocket   │
         │ (bound socket) │
         └───────┬────────┘
                 │
         ┌───────▼────────┐
         │  Socket        │
         │  (wrappers)    │
         └───────┬────────┘
                 │
         ┌───────▼────────────────────┐
         │ OS Socket Functions        │
         │ (socket, bind, listen,     │
         │  accept, recv, send, etc)  │
         └────────────────────────────┘
```

**Key Classes**:
- `Socket` - Generic socket wrapper
- `ListenSocket` - Socket bound to a port, listening for connections

### 5. **HTTP System** (Protocol Handling)
**Files**:
- `incl/HttpRequest.hpp` (STUB - needs implementation)
- `incl/HttpResponse.hpp`, `src/Http/HttpResponse.cpp`
- `incl/HttpRequest.hpp`, `src/Http/HttpRequest.cpp` (STUB)

**Purpose**: Parse HTTP requests and generate HTTP responses

**Current Status**:
- ✓ HttpResponse: Fully implemented
- ⏳ HttpRequest: Needs implementation

### 6. **CGI System** (Script Execution)
**Files**:
- `incl/Cgi/CgiHandler.hpp`, `src/CgiHandler/CgiHandler.cpp`
- `incl/Cgi/ErrorPageHandler.hpp`, `src/CgiHandler/ErrorPageHandler.cpp`

**Purpose**: Execute external scripts and serve error pages

### 7. **Configuration System** (Server Setup)
**Files**: `incl/Config.hpp`, `src/Config/Config.cpp`

**Purpose**: Parse configuration files and store settings

**Current Status**: STUB - needs implementation

---

## Data Flow Example: Client Request

Let's trace what happens when a browser visits `http://localhost:8080/`:

```
┌──────────────┐
│   Browser    │
│ connects to  │
│ localhost:80 │
└──────┬───────┘
       │ (TCP SYN)
       ▼
┌──────────────────────────────┐
│ Listening Socket (port 8080) │
│ waiting in epoll             │
└──────┬───────────────────────┘
       │ epoll detects new connection
       ▼
┌───────────────────────────────────┐
│ Server::run()                     │
│ events = epoll.wait()  ← returns! │
│ event.fd = listening_socket       │
│ event.isWeird() = false           │
│ fd == listening_socket? YES!      │
│ → Call newClient()                │
└───────┬─────────────────────────┬─┘
        │                         │
        │ socket.accept()         │ Creates new client fd (4)
        │                         │
        ▼                         ▼
    ┌─────────────────────────────────┐
    │ epoll.ctl(Add, fd=4)            │
    │ Now monitoring client socket 4  │
    └─────────────────────────────────┘
        │
        │ epoll.wait() blocks again
        │
┌──────────────────────────────────────┐
│ Browser sends:                       │
│ GET / HTTP/1.1\r\n                  │
│ Host: localhost:8080\r\n            │
│ \r\n                                 │
└──────┬───────────────────────────────┘
       │ (TCP data)
       ▼
┌───────────────────────────────────┐
│ Server::run()                     │
│ events = epoll.wait()  ← returns! │
│ event.fd = 4 (client)             │
│ event.isWeird() = false           │
│ fd == listening_socket? NO        │
│ → Call existingClient(fd=4)       │
└───────┬─────────────────────────┐─┘
        │                         │
        │ recv(fd=4)              │ Reads request
        │ request =               │
        │ "GET / HTTP/1.1\r\n..." │
        │                         │
        ▼                         ▼
    ┌──────────────────────────────┐
    │ Parse request (temporary)    │
    │ method = "GET"               │
    │ path = "/"                   │
    │ version = "HTTP/1.1"         │
    └──────┬───────────────────────┘
           │
           ▼
    ┌──────────────────────────────┐
    │ Check if CGI request?        │
    │ path = "/" → NO              │
    │ Try to read file             │
    │ readFile("./webpages/...")   │
    └──────┬───────────────────────┘
           │
           ▼
    ┌──────────────────────────────┐
    │ Create HttpResponse(200)     │
    │ setBody(file content)        │
    │ toString() → HTTP response   │
    └──────┬───────────────────────┘
           │
           ▼
    ┌──────────────────────────────┐
    │ send(fd=4, response)         │
    │ Browser receives HTML        │
    └──────────────────────────────┘
           │
           ▼
    ┌──────────────────────────────┐
    │ close(fd=4)                  │
    │ epoll.ctl(Del, fd=4)         │
    │ Stop monitoring this client  │
    └──────────────────────────────┘
```

---

## Component Interaction Matrix

| Component | Uses | Is Used By | Purpose |
|-----------|------|-----------|---------|
| Epoll | Event | Server | Monitor file descriptors |
| Event | (none) | Epoll, Server | Represent I/O events |
| Server | Epoll, Socket, CgiHandler | main.cpp | Main logic & routing |
| ListenSocket | Socket | Server | Create listening socket |
| Socket | Wrappers | ListenSocket, Server | OS socket functions |
| HttpResponse | (none) | Server | Build responses |
| HttpRequest | (none) | Server (TODO) | Parse requests |
| CgiHandler | (none) | Server | Execute scripts |
| ErrorPageHandler | (none) | Server, CgiHandler | Generate error pages |

---

## Current Implementation Status

### ✓ Complete
- Epoll monitoring system
- Socket wrapper layer
- HTTP response generation
- CGI script execution
- Error page handling
- Basic event loop

### ⏳ In Progress
- Static file serving (partially done)
- Client state management (missing)

### ⭕ TODO
- HttpRequest parser (teammate's responsibility)
- Configuration file parser
- Virtual host support
- Non-blocking CGI execution with epoll
- POST request body handling
- Redirect handling
- Auto-indexing

---

## Key Design Decisions

### 1. **Event-Driven Architecture**
- Why: Scales to thousands of clients efficiently
- How: Epoll monitors all sockets, returns only active ones
- Benefit: Single-threaded, no context switching overhead

### 2. **Stateless Request Handling**
- Current: Each request is handled immediately
- Problem: Long requests come in chunks, need buffering
- TODO: Implement per-client state (buffer, partial request, CGI pipes)

### 3. **Wrapper Classes**
- Why: C socket API is error-prone and non-idiomatic C++
- How: Socket, ListenSocket wrap raw syscalls
- Benefit: RAII (automatic cleanup), error checking

### 4. **Separation of Concerns**
- Server: Logic & routing
- Socket: Network I/O
- HttpResponse: Protocol formatting
- CgiHandler: External script execution
- Each has ONE responsibility
