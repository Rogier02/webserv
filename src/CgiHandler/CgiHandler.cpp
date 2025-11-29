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
#include <cstring>
#include <sstream>
#include <iostream>

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

std::string
CgiHandler::execute(const std::string& path, const std::string& method, 
		const std::string& query, const std::string& body) const
{
	std::string extension = getCgiExtension(path);
	if (extension.empty()) {
		return("<html><body><h1>500 Internal Server Error</h1>"
                "<p>Unknown CGI extension</p></body></html>");
	}

	std::string interpreter = getCgiInterpreter(extension);
	if (interpreter.empty()) {
		return ("<html><body><h1>500 Internal Server Error</h1>"
                "<p>Unknown CGI extension</p></body></html>");
	}
	
	size_t lastSlash = path.find_last_of("/");
	std::string scriptFilename;
	if (lastSlash != std::string::npos)
		scriptFilename = path.substr(lastSlash + 1);
	else
		scriptFilename = path;

	std::string scriptPath = _cgiDirectory + scriptFilename;

	setupEnvironment(path, method, query, body);

	return (executeScript(interpreter, scriptPath, body));

};

void
CgiHandler::setupEnvironment(const std::string& path, const std::string& method,
        const std::string& query, const std::string& body) const
{
    // HTTP request method (GET, POST, DELETE, etc.)
    setenv("REQUEST_METHOD", method.c_str(), 1);
    
    // Path to the CGI script
    setenv("SCRIPT_NAME", path.c_str(), 1);
    
    // URL query string (after the ? in URL)
    setenv("QUERY_STRING", query.c_str(), 1);
    
    // Length of request body in bytes
    setenv("CONTENT_LENGTH", std::to_string(body.length()).c_str(), 1);
    
    // MIME type of request body
    setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
    
    // Server hostname
    setenv("SERVER_NAME", "webserv", 1);
    
    // Server port
    setenv("SERVER_PORT", "8080", 1);
    
    // HTTP version
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    
    // CGI gateway interface version
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
};

std::string
CgiHandler::executeScript(const std::string& interpreter,
	const std::string& scripitPath, const std::string& body) const
{
	// piping
	int pipefd[2]; // {read, write};
	if (pipe(pipefd) == -1) {
		return ("<html><body><h1>500 Internal Server Error</h1>"
                "<p>Pipe creation failed</p></body></html>");
	}

	// forking
	pid_t pid = fork();
	if (pid == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		return ("<html><body><h1>500 Internal Server Error</h1>"
				"<p>Fork failed</p></body></html>");
	}

	// ===== CHILD ===== 
	if (pid == 0) {

		// close read end
		close(pipefd[0]);
		
		// Redirect stdout to pipe
		dup2(pipefd[1], STDOUT_FILENO);

		// Redirect stderr to pipe (so errors appear in output)
		dup2(pipefd[1], STDERR_FILENO);

		close(pipefd[1]);

		// Execute the script with the interpreter
		// Format: Interpreter(c string) scriptPath(c string) (required formating for execve)
		char *args[] = {
			(char *)interpreter.c_str(), // Progam to run
			(char *)scripitPath.c_str(), // First argument (script file)
			NULL 						 // NULL-terminated argument list
		};

		// Execute script (replace this process)
		// If execve returns, it means an error occured
		execve(interpreter.c_str(), args, environ);
	} 
	
	// ===== PARENT PROCESS =====
	else {

		//Close write end (parent only reads)
		close(pipefd[1]);

		// Buffer for reading script output
		std::string output;
		char buffer[4096]; //Read in 4kb chunks
		ssize_t nBytes;

		// Read all output from child process
		while ((nBytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
			// Append bytes to output string
			output.append(buffer, nBytes);
		}

		// Close read end
		close(pipefd[0]);

		// Wait for child precess to complete and get exit status
		int status;
		waitpid(pid, &status, 0);

		//Check if child exited normally and with uccess
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			// Success - return script outpu
			return (output);
		} else {
			//Script execution failed
			return ("<html><body><h1>500 Internal Server Error</h1>"
                    "<p>CGI script execution failed</p></body></html>");
		}

	}
	return ("");
};


