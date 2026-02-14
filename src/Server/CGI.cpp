#include "CGI.hpp"
#include "Http_v1_0.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <sstream>
#include <iostream>


bool
CGI::isCgiRequest(std::string const &path) const
{
		for(const auto& ext : SupportedExtensions)
		{
			if (path.find(ext.first) != std::string::npos)
				return (true);
		}
		return (false);
}

std::string
CGI::getCgiExtension(std::string const &path) const
{
	for (const auto& ext : SupportedExtensions) 
	{
		if (path.find(ext.first) != std::string::npos)
			return (ext.first);
	}
	return ("");
}

std::string
CGI::getCgiInterpreter(std::string const &extension) const
{
	auto it = SupportedExtensions.find(extension);
	if (it != SupportedExtensions.end())
		return(it->second);
	return ("");
}

std::string
CGI::execute(Http::Request& request)
{
	std::string path = request.getURI();
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

	std::string scriptPath = BinDirectory + scriptFilename;

	char **env = setupEnvironment(request);
	std::string output = executeScript(interpreter, scriptPath, request.getEntityBody(), env);

	// Write funciton to clean up env
	return (output);

};

//TODO: REMOVE SETENV and add own implamentation
char **
CGI::setupEnvironment(Http::Request& request)
{
	std::vector<std::string> _envVariables;

	_envVariables.push_back("REQUEST_METHOD=" + request.getMethod());
	_envVariables.push_back("CONTENT_TYPE=" + request.getRequestHeaderValue("Content-Type"));
	_envVariables.push_back("CONTENT_LENGTH=" + request.getRequestHeaderValue("Content-Length"));
	_envVariables.push_back("SCRIPT_NAME=" + request.getScriptName());
	_envVariables.push_back("QUERY_STRING=" + request.getQueryString());
	_envVariables.push_back("REQUEST_URI=" + request.getURI());
	_envVariables.push_back("SERVER_PROTOCOL=" + request.getVersion()); //check of we 0.9 en 1.0 ook kunnen parsen
	_envVariables.push_back("GATEWAY_INTERFACE=CGI/1.1"); // wordt dit geparsed uit de httpRequest? of is dit vast?
	_envVariables.push_back("SERVER_NAME=webserv"); //nog niet duidelijk hoe je hieraan komt.
	_envVariables.push_back("SERVER_PORT=" + request.getHost("Port"));
	_envVariables.push_back("SERVER_ADDR=" + request.getHost("Address"));

	// add HTTP headers as CGI variables

	const Http::HeaderMap	&headers = request.getRequestHeaders();
	for (Http::HeaderMap::const_iterator it = headers.begin(); it != headers.end(); ++it)
{
	const std::string& headerName  = it->first;
	const std::string& headerValue = it->second;
		if (headerName == "Content-Type" || headerName == "Content-Length" || headerName == "Host")
			continue;

		std::string cgiVarName = "HTTP_";
		for (char c : headerName) {
			if (c == '-')
				cgiVarName += '_';
			else
				cgiVarName += std::toupper(c);
		}
		_envVariables.push_back(cgiVarName + "=" + headerValue);
	}

	char **env = new char*[_envVariables.size() + 1];
	for (size_t i = 0; i < _envVariables.size(); i++) {
		env[i] = new char[_envVariables[i].length() + 1];
		std::strcpy(env[i], _envVariables[i].c_str());
	}
	env[_envVariables.size()] = NULL;

	return (env);
};

// TODO: CONVERT MAP TO CHAR **

std::string
CGI::executeScript(
	const std::string &interpreter,
	const std::string &scripitPath,
	const std::string &requestBody,
	char **env)
{
	// Bidirectiona; pipes stdin for child stdout for child
	int stdinPipe[2]; // Parent writes, child reads
	int stdoutPipe[2]; // Child writes, parent reads

	if (pipe(stdinPipe) == -1 || pipe(stdoutPipe) == -1) {
		return ("<html><body><h1>500 Internal Server Error</h1>"
				"<p>Pipe creation failed</p></body></html>");
	}
	
	// forking
	pid_t pid = fork();
	if (pid == -1) {
		close(stdinPipe[0]);
		close(stdinPipe[1]);
		close(stdoutPipe[0]);
		close(stdoutPipe[1]);
		return ("<html><body><h1>500 Internal Server Error</h1>"
					"<p>Fork failed</p></body></html>");
	}
	// ===== CHILD =====
	if (pid == 0) {
		
		close(stdinPipe[1]); // close write end of stdin pipe
		close(stdoutPipe[0]); // close read end of stdout pipe

		// Redirect stdint to read from pipe
		dup2(stdinPipe[0], STDIN_FILENO);
		close(stdinPipe[0]);

		// Redirect stdout to write to pipe
		dup2(stdoutPipe[1], STDOUT_FILENO);
		// Redirect stderr to stdout (so errors appear in response)
		dup2(stdoutPipe[1], STDERR_FILENO);
		close(stdoutPipe[1]);

		// Execute the script with the interpreter
		// Format: Interpreter(c string) scriptPath(c string) (required formating for execve)
		char *args[] = {
			(char *)interpreter.c_str(), // Progam to run
			(char *)scripitPath.c_str(), // First argument (script file)
			NULL,						 // NULL-terminated argument list
		};

		// Execute script (replace this process)
		// If execve returns, it means an error occured
		execve(interpreter.c_str(), args, env);

		std::cerr << "exec failed for: " << interpreter << std::endl;
		exit(1);
	}

	// ===== PARENT PROCESS =====
	else {
		// Close unused ends
		close(stdinPipe[0]);
		close(stdoutPipe[1]);

		// Write request body to childs stdin (for POST request)
		if (!requestBody.empty()) {
			ssize_t bytesWritten = write(stdinPipe[1], requestBody.c_str(), requestBody.length());
			if (bytesWritten == -1) {
				std::cerr << "Failed to write to CGI stdin" << std::endl;
			}
		}
		close(stdinPipe[1]); // Close write end after writing

		// Read all output form child process
		std::string output;
		char buffer[4096];
		ssize_t nBytes;

		while ((nBytes =  read(stdoutPipe[0], buffer, sizeof(buffer))) > 0) {
			output.append(buffer, nBytes);
		}
		close(stdoutPipe[0]);

		// Wait for child process to complete
		int status;
		waitpid(pid, &status, 0);

		// Check if child exited normally
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			// Success - parse and return CGI response
			return (parseCgiResponse(output));
		} else {
			return ("<html><body><h1>500 Internal Server Error</h1>"
                    "<p>CGI script execution failed</p></body></html>");
		}
	}
	return ("");
}


// /*
// -- HTTP1.0 request example --
// GET /index.html HTTP/1.0
// Host: www.example.com
// User-Agent: Mozilla/5.0
// Accept: text/html
// */

std::string
CGI::parseCgiResponse(const std::string &rawOutput) const
{
	//CGI scripts output headers followed by blank line, then body

	size_t headerEndPos = rawOutput.find("\r\n\r\n");
	if (headerEndPos == std::string::npos) {
		// No headers found, treats entire output as body
		return (rawOutput);
	}

	std::string headers = rawOutput.substr(0, headerEndPos);
	std::string body = rawOutput.substr(headerEndPos + 4);

	  // TODO: Parse headers and add them to HTTP response
    // For now, just return the body
    // In a complete implementation, you would:
    // 1. Parse Status header (e.g., "Status: 200 OK")
    // 2. Extract other CGI headers
    // 3. Build proper HTTP response with these headers

	return (body);
}
