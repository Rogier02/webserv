/**
 * @file CgiHandler.cpp
 * @brief Implementation of CGI script execution handler
 * 
 * Responsiblities:
 * - Parse incoming requests to identify CGI scripts 
 * - Set up secure exection environment with proper variables
 * - Fork and execute external scripts safely
 * - Capture an return script output
 * - Handle errors gracefully
 * 
 * Security Considerations:
 * - Scripts are restricted to ./src/cgi-bin/ directiory
 * - Env variables are validated
 * - Child processes are cleaned properly
 * - Output is caputed and sanitized
 */

#include "CgiHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iostream>

extern char **environ;
/**
 * Initializing static members:
 * Maps file extensions to the interpreters
 * 
 * Format: {".extension" (e.g., ".py"), "/path/to/interpreter"}
 * 
 * Currently ".php" & ".pl" are not supported
 */
const std::map<std::string, std::string> CgiHandler::_cgiExtensions = {
	{".py", "/usr/bin/python3"},
	{".sh", "/bin/bash"}
	//{".php", "/usr/bin/php-cgi"},
	// {".pl", "/usr/bin/perl"}
};

const std::string CgiHandler::_cgiDirectory = "./src/cgi-bin/";

CgiHandler::CgiHandler()
{}

CgiHandler::~CgiHandler()
{}


bool
CgiHandler::isCgiRequest(const std::string& path) const
{
	// Loop through possible extensions
	for(const auto& ext : _cgiExtensions) {
		// Check if extension matches
		if (path.find(ext.first) != std::string::npos)
			return (true); // Found a CGI extension
	}
	return (false); // No Cgi extension found
}

std::string
CgiHandler::getCgiExtension(const std::string&  path) const 
{
	// Check each registered extension 
	for (const auto& ext : _cgiExtensions) {
		// Look for extension in path
		if (path.find(ext.first) != std::string::npos)
			return (ext.first); // Returns found extension
	}
	return (""); // Extension not ofund
};

std::string
CgiHandler::getCgiInterpreter(const std::string& extension) const
{
	// Search for extension in the map
	auto it = _cgiExtensions.find(extension);

	// If found, return interperter path
	if (it != _cgiExtensions.end())
		return(it->second);

	// Not found
	return ("");
};

//TODO: REMOVE SETENV and add own implamentation
void
CgiHandler::setupEnvironment(const std::string& path, const std::string& method,
								const std::string& query, const std::string& body) const
{
	// basic required
	setenv("REQUEST_METHOD", method.c_str(), 1);
	setenv("SCRIPT_NAME", path.c_str(), 1);
	setenv("QUERY_STRING", query.c_str(), 1);
	setenv("CONTENT_LENGTH", std::to_string(body.length()).c_str(), 1);

	// additionals
	setenv("CONTENT_TYPE", "text/plain", 1);
	setenv("SERVER_NAME", "localhost", 1);
	setenv("SERVER_PORT", "8080", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
};

CgiHandler::CgiProcess
CgiHandler::executeAsync(const std::string& path, const std::string& method,
					const std::string& query, const std::string& body) const
{
	// Validate request
	std::string extension = getCgiExtension(path);
	if (extension.empty())
		throw std::runtime_error("Unknown CGI extension");
	
	std::string interpreter = getCgiInterpreter(extension);
	if(interpreter.empty())
		throw std::runtime_error("Unsupported CGI extension");

	// Extract script filename
	size_t lastSlash = path.find_last_of("/");
	std::string scriptFilename = (lastSlash != std::string::npos)
		? path.substr(lastSlash + 1)
		: path;

	std::string scriptPath = _cgiDirectory + scriptFilename;

	// Create pipe Before fork
	int pipefd[2];
	if (pipe(pipefd) == -1)
		throw std::runtime_error("pipe() failed");

	// Fork child process
	pid_t pid = fork();

	if (pid == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		throw std::runtime_error("fork() failed");
	}

	if (pid == 0) {
		// CHILD PROCESS
		close(pipefd[0]); // Close read end

		// Redirect stdout/stderr to pipe
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		close(pipefd[1]);

		setupEnvironment(path, method, query, body);

		// Execute script
		char *args[] = {
			(char *)interpreter.c_str(),
			(char *)scriptPath.c_str(),
			NULL
		};

		execve(interpreter.c_str(), args, environ);
		exit(1);
	}

	// PARENT
	close(pipefd[1]);

	// Make pipe nonblocking
	int flags = fcntl(pipefd[0], F_GETFL, 0);
	fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

	return {pid, pipefd[0]};
}
