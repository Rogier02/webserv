#ifndef	CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <string>
#include <map>
#include <vector>

/**
 * @class CgiHandler
 * @brief Manages Common Gateway Interface (CGI) script execution
 * 
 * This class is responsible for:
 * - Detecting CGI request by file extension
 * - Setting up the CGI execution environment
 * - FOrking Child processes to execute scripts
 * - Capturing script output and returning it as HTTP response body
 * 
 * Current extensions: .py (Python3), .sh (Bash)
 * 
 * @note Enironment variables are set according the CGI/1.1 specifications (RFC 3875)
 */

class CgiHandler
{
	public:
		CgiHandler();
		~CgiHandler();

		/**
		 * @struct CgiProcess
		 * @brief Info about a running CGI process
		 */
		struct CgiProcess {
			pid_t	processId;		// Child process ID
			int		outputPipeFd;	// File des to read output from
		};

	private:
		/**
		 * @brief Map of file extensions to their interpreters
		 * 
		 * Example mappings:
		 * - ".py" -> "usr/bin/python3"
		 * - ".sh" -> "bin/bash"
		 * 
		 * @note This may or may not be overkill. 
		 * Depends wether or not handling multiple extensions is relevant or important
		 * 
		 */
		static const std::map<std::string, std::string> _cgiExtensions;

		/**
		 * @brief Root directory where CGI scripts are stored
		 * 
		 * Scripts are expected to be in: ./src/cgi-bin/
		 * Example ./src/cgi-bin/cgi.py
		 */
		static const std::string _cgiDirectory;

	public:
		/**
		 * @brief Check if a request path points to a CGI script
		 * 
		 * @param path The request path (e.g., "/cgi.py" or "upload.sh")
		 * @return true if path contains a registered CGI extension, false otherwise
		 * 
		 * @example 
		 * CgiHandler handler;
		 * handler.isCgiRequest("/cgi.py");		<- Returns true
		 * handler.isCgiRequest("/index.html"); <- Returns false
		 */
		bool			isCgiRequest(const std::string& path) const;

		/**
		 * @brief Start CGI script execution (non-blocking (hopefully))
		 * 
		 * Forks child process but doesnt wait.
		 * Returns process info to Epoll loop
		 * 
		 * @param path The request path/script filename
		 * @param method HTTP method (GET, POST, etc.)
		 * @param query Query string from URL (e.g., "param1=value1&param2=value2")
		 * @param body Request body (for POST requests)
		 * 
		 * @return CgiProcess with pid and output pipe fd
		 * 
		 * @throws std::runtime_error on fork/pipe failure
		 */
		CgiProcess		executeAsync(const std::string& path, 
								const std::string& method,
								const std::string& query, 
								const std::string& body) const;

	private:
		/**
		 * @brief Extract file extension from path
		 * 
		 * @param path Full file path or filename
		 * @return File extension (e.g., ".py", ".sh") or empty string if not found
		 * 
		 * @example 
		 * getCgiExtension("/path/to/script.py");	<- Returns ".py"
		 * getCgiExtension("unknow.txt");			<- Returns ""
		 */
		std::string		getCgiExtension(const std::string& path) const;

		/**
		 * @brief Get interpreter path for a given extension
		 * 
		 * @param extension File extension (e.g., ".py")
		 * @return Full path to interpreter or empty string if not supported
		 * 
		 * @example
		 * getCgiInterpreter(".py");	<- Returns "/usr/bin/python03"
		 * getCgiInterpreter(".php");	<- Returns "" (Not supported)
		 */
		std::string		getCgiInterpreter(const std::string& extension) const;

		/**
		 * @brief Set up environment variables for CGI script execution
		 * 
		 * Variables set (according to CGI/1.1 RFC 3875):
		 * - REQUEST_METHOD: HTTP metod (GET, POST, etc.)
		 * - SCRIPT_NAME: Script path
		 * - QUERY_STRING: URL query parameters
		 * - CONTENT_LENGTH: Size of request body
		 * - CONTENT_TYPE: Request content type
		 * - SERVER_NAME: Server Hostname
		 * - SERVER_PORT: Server port
		 * - SERVER_PROTOCAL: HTTP version
		 * - GATEWAY_INTERFACE: CGI version (CGI/1.1)
		 * 
		 * @param path Script path
		 * @param method HTTP method 
		 * @param query URL query string 
		 * @param body Request body content
		 * 
		 * @note Uses setenv() <- This is illegal and needs to be replaced / Also not thread safe
		 * @note Enviroment is inherited by child process
		 * 
		 */
		void			setupEnvironment(const std::string& path, const std::string& method,
								const std::string& query, const std::string& body) const;

};

#endif