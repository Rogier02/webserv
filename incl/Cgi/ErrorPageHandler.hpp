#ifndef ERRORPAGEHANDLER_HPP
# define ERRORPAGEHANDLER_HPP

#include <string>
#include <map>

/**
 * @class ErrorPageHandler
 * @brief Manages HTTP error pages for the server 
 * 
 * Responsibilities
 * - Load custom error pages from files
 * - Generate defualt error pages if custom ones not available
 * - Return appropriate error page HTML for any HTTP error code 
 * - Support dynamic error page configuration
 * 
 * Error pages can be:
 * 1. Custom: Loaded from files specified in configuration
 * 2. Default: Generated with standard HTML format
 * 
 * @example
 * ErrorPageHandler handler;
 * handler.setErrorPage(404, "./webpages/404.html");
 * std::string page = handler.getErrorPage(404); 
 * 
 * @note Defual pages are generated if custom ones fail to load
 * @note This class is thread-safe after construction
 * 
 */
class ErrorPageHandler
{
	public:
		/** @brief constructor - initializes handler */
		ErrorPageHandler();

		/** @brief destructor */
		~ErrorPageHandler();

	private:
		/**
		 * @brief Root director for error page files
		 * 
		 * Error page files should be stored in: ./webpages/
		 * Example files:
		 * - ./webpages/400.html (Bad Request)
		 * - ./webpages/404.html (Not Found)
		 * - ./webpages/500.html (Internal Server Error)
		 */
		static const std::string _errorPageDirectory;

		/**
		 * @brief Cache of loaded error pages
		 * 
		 * Maps HTTP status code (int) to HTML content (string)
		 * Example
		 * - 404 -> "<html>...404 page HTML... </html>"
		 * - 500 -> "<html>...500 page HTML...</html>"
		 * 
		 * @note Only stores custom pages that were successfully loaded
		 * @note Default pages are generated on-the-fly if not in cache
		 */
		std::map<int, std::string> _errorPages;

	public:
		/**
		 * @brief Get error page HTML for a given status code
		 * 
		 * Returns custom error page if available, otherwise generates default
		 * 
		 * Priority order:
		 * 1. Custom page if loaded via setErrorPage()
		 * 2. Default generated page with proper styling
		 * 
		 * @param statusCode HTTP status code (e.g., 404, 500)
		 * @return HTML string ready to send as HTTP response body
		 * 
		 * @note Always returns valid HTML, never throws execption
		 * @note Return value can be directly used in HttpResponse
		 * 
		 * @example
		 * std::string html = handler.getErrorPage(404);
		 * response.setbody(html);
		 * response.setStatus(404);
		 */
		std::string getErrorPage(int statusCode) const;

		/**
		 * @brief Load a custom error page from file
		 * 
		 * Attempts to load and cache error page HTML from file system
		 * 
		 * @param statusCode HTTP status code (e.g., 404, 500)
		 * @param filePath to custon error page file
		 * 					(e.g., "./webpages/404.html")
		 * 
		 * @note If file doesn't exist ir can't be read, silently skips
		 * 		(will fall back to default page)
		 * @note File is read entirely into memory
		 * @note Caches loaded page for future requests
		 * 
		 * @example
		 * handler.setErrorPage(404, "./webpages/404.html");
		 * handler.setErrorPage(500, "./webpages/500.html");	
		 */
		void		setErrorPage(int statusCode, const std::string& filePath);

		/**
		 * @brief Generate a defaul error page HTML
		 * 
		 * Creates a basic error page with:
		 * - HTML structure
		 * - Status code and reasonPhrase
		 * - Server id
		 * - Horizontal rule for visual seperation
		 * 
		 * Supported codes:
		 * - 400: Bad Request
		 * - 404: Page Not Found
		 * - 500: Internal Server Error
		 * - 502: Bad Gateway
		 * - 503: Service Unavailable
		 * 
		 * @param statusCode HTTP status code
		 * @return Generated HTML error page
		 * 
		 * @note This is calle dautomatically by getErrorPage() if needed
		 * @note Returns valid HTML even for unkown codes
		 * 
		 * @example Generated html:
		 * <html>
         * <head><title>404 Not Found</title></head>
         * <body>
         * <center><h1>404 Not Found</h1></center>
         * <hr><center>webserv/1.0</center>
         * </body>
         * </html>
		 */
		std::string	getDefaultErrorPage(int statusCode) const;

	private:
		/**
		 * @brief Load error page HTML from file
		 * 
		 * Reads entire file ocntents into string
		 * 
		 * @param filePath Path to HTML file
		 * @return File contents as string or empty string if read fails
		 * 
		 * @note Does not throw exception on error
		 * @note Returns empty string if file not found
		 * @note File must be readable by current process
		 * 
		 * @example
		 * std::string content = loadErrorPageFromFile("./webpages/404.html");
		 * if (content.empty())
		 * 		// File couldn't be read
		 */
		std::string loadErrorPageFromFile(const std::string& filePath) const;
		
};

#endif