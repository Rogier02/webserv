# 42 Webserv
Welcome to our webserv project!


## Introduction
The aim of this project was to create an HTTP server using c++ in a Non-Blocking manner.

The project consists of multiple parts. The parser handles the processing of incomming requests and the configuration file for the HTTP server. The core implamentation of the HTTP server and client handling and connections. And the execution using (CGI) common gateway interface. 


## Project Overview

Our HTTP server will begin by parsing the configuration file that is needed to set the parameters for running the HTTP server. This will determin what and how many ports the server will be listening to. Additionally the configuration will also include the paths for all of the file locations for the CGI scripts, upload locations, index pages, error pages, etc.

Once the HTTP server is configured without error. The server with 'run' and monitor incomming connections using epoll().
As connection are made the server will identify the type of connection and parse requests. Based on the request the server will then execute with CGI a response or generate a simple HTTP response to send back to the client. 

## Requirements

	- The use of 1 poll(), epoll() or equivalent loop.
	- The poll()/epoll() loop must monitor all communication between the server and clients.
	- Not allowed to check the value of errno for the purposes of adjusting server behaviour.
	- Must use at least GET, POST and DELETE methods.
	- Provide default error pages.
	- Clients must be able to upload files.
	- Server must be able to listen to multiple ports.

## Install & Setup

## Usage

## Architecture

## Resources

