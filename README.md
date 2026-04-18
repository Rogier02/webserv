# 42 Webserv

Building a C++ HTTP server

## Contributors

[SimonvH03](https://github.com/SimonvH03) - [Laura-dekoning](https://github.com/laura-dekoning) - [Rogier02](https://github.com/Rogier02)


## Introduction

The aim of this project was to create an HTTP server using c++ in a Non-Blocking manner.

The primary functions are to store, process and deliver web pages to clients.

## Requirements

	> The Http server is not allowed to crash under any circumstances

	> Must use a configuration file 

	> Must remain non-blocking at all times and handle client connects and disconnects

	> Use of only one poll() or equivalent function is allowed

	> Use of NGINX is allowed to comapre headers and anser behaviours

	> Accurate HTTP response status codes

	> Must accomodate at least GET, POST, DELETE methods

	> Must be able to listen to multiple ports simultaniously

## Features

	> Complys with HTTP/1.0

	> CGI (Common gateway interface) for executing programs.

	> Upload and delete files 

	> Timestamps

	> Custom error pages

	> NGINX-like webserver configuration

## Install & Setup

Note: this setup is specifically for Ubuntu linux. Compiling and running this software on other distro's or operating systems will likely result in error. 

We did not include any docker-compose file or containers as it was not a requirement for this project. 

### Building and running

To build first clone the repo and run make inside the root directory:

```sh
make
```

Once its compiled run the executable:

```sh
./webserv
```

### Clean up 

remove objects:

```sh
make clean
```

Remove all artifacts created by building:

```sh
make fclean
```

Cleanup and rebuild

```sh
make re
```

## Resources

[Subject Pdf](https://github.com/Rogier02/webserv/blob/main/devdocs/webserv.pdf)
[Beej's Guide to network Programming ](https://beej.us/guide/bgnet/)

### RFC
	
[RFC 1945 - HTTP PROTOCOL](https://datatracker.ietf.org/doc/html/rfc1945)
[RFC 7230 - HTTP PROTOCOL](https://datatracker.ietf.org/doc/html/rfc7230)
[RFC 6455 - WEBSOCKETS](https://datatracker.ietf.org/doc/html/rfc6455)
[RFC 3875 - CGI](https://datatracker.ietf.org/doc/html/rfc3875)
