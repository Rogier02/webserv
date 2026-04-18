# 42 Webserv

Building a C++ HTTP server


## Contributors

[SimonvH03](https://github.com/SimonvH03) - [Laura-dekoning](https://github.com/laura-dekoning) - [Rogier02](https://github.com/Rogier02)


## Introduction

The aim of this project was to create an HTTP server using c++ in a non-blocking manner.

The primary functions are to store, process and deliver web pages to clients.


## Requirements

- The HTTP server is not allowed to crash under any circumstances

- Must use a configuration file 

- Must remain non-blocking at all times and handle client connects and disconnects

- Use of only one poll() or equivalent function is allowed

- Use of NGINX is allowed to comapre headers and answer behaviours

- Accurate HTTP response status codes

- Must accomodate at least GET, POST, DELETE methods

- Must be able to listen to multiple ports simultaniously


## Features

- Complies with HTTP/1.0 According to the [RFC 1945](https://datatracker.ietf.org/doc/html/rfc1945)

- CGI (Common gateway interface)

- Upload and delete files 

- Timestamps

- Custom error pages

- NGINX-like webserver configuration


## Install & Setup

Note: this setup is specifically for Ubuntu linux. Compiling and running this software on other distro's or operating systems can result in error. 

We did not include any docker-compose file or containers as it was not a requirement for this project. 


### Building and running

To build first clone the repo and run make inside the root directory:

```sh
make
```

Once it's compiled, run the executable:

```sh
./webserv
```

Then in your browser go to:

```sh
localhost:8080
```

*The port may be different depending on the configuration used. But by default it should be 8080.


### Clean up 

Remove objects:

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

[MDN web docs on HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)
	
[RFC 1945 - HTTP/1.0](https://datatracker.ietf.org/doc/html/rfc1945)

[RFC 6455 - Websockets](https://datatracker.ietf.org/doc/html/rfc6455)

[RFC 3875 - CGI](https://datatracker.ietf.org/doc/html/rfc3875)

