#include "ClientEvent.hpp"

const std::string	ClientEvent::HeaderEnd = Http::CRLF + Http::CRLF;

ClientEvent::ClientEvent(int socketFd, Epoll &epoll, Config::Listener const &config)
	:	Event(socketFd, Epoll::Events::In, epoll, config)
	,	_receivedHead(false)
	,	_cgild(-1)
{
	std::cout << "Client " << data.fd << " \e[34mConstructed\e[0m\n";
}

ClientEvent::~ClientEvent()
{
	std::cout << "Client " << data.fd << " \e[34mDestructed\e[0m\n";
}

void
ClientEvent::_in()
{
	try	{
		if (Socket::recv(data.fd, _requestBuffer) == 0)
			return (EventHandlers::erase(data.fd));
		if (!_receivedHead)
			_receiveHead();
		if (_receivedHead)
			_receiveBody();
	} catch (HttpError const &e) {
		u_int16_t	statusCode = e.status();
		std::cerr << "\e[31mError:\e[0m " << e.what() << std::endl;
		LOG(Error, e.what());
		_response.err(statusCode);
		if (r_config.errorPages.contains(statusCode)) {
			const std::string errorPage	= r_config.errorPages.at(statusCode);
			const std::string content	= IO::getFileContent(errorPage);
			if (!content.empty())
				_response.setEntityBody(content, errorPage);
			EasyPrint(errorPage);
			EasyPrint(content);
		}
		_finalise();
		// return;
	}
}

void
ClientEvent::_out()
{
	::ssize_t	sent = Socket::send(data.fd, _responseBuffer);

	if (sent == 0)
		return;

	_responseBuffer.erase(0, sent);

	if (_responseBuffer.empty()) {
		LOG(Info, "Client " + std::to_string(data.fd) + " Completed Response");

		std::cout << "Client " << data.fd << " \e[32mCompleted Response\e[0m\n";
		EventHandlers::erase(data.fd);
	}
}

void
ClientEvent::_receiveHead() {
	const ::size_t headerEndPos = _requestBuffer.find(HeaderEnd);

	if (headerEndPos == std::string::npos)
		return;

	_receivedHead = true;

	std::string	head(_requestBuffer.substr(0, headerEndPos));
	_requestBuffer.erase(0, headerEndPos + HeaderEnd.length());

	if (_request.parseHead(head) == -1)
		throw HttpError(400);

	LOG(Debug, "Client " + std::to_string(data.fd) + " Received Good Head");
}

void
ClientEvent::_receiveBody() {
	int	bytesSet = _request.setEntityBody(_requestBuffer);

	EasyPrint(bytesSet);
	if (bytesSet == -1)
		throw HttpError(400);
	if (bytesSet == 0)
		return;

	LOG(Debug, "Client " + std::to_string(data.fd) + " Received Full Body");
	EasyPrint(_requestBuffer);

	_processRequest();

	EasyPrint(_cgild);
	(_cgild == -1)
	?	_finalise()
	:	_mod(0)
	;

	LOG(Debug, "Client " + std::to_string(data.fd) + " Processed Request: "
		+ ((_cgild == -1) ? "ready to send" : "waiting for CGI"));
}

void
ClientEvent::_finalise()
{
	_responseBuffer = _response.toString();
	_mod(Epoll::Events::Out);
}

std::string
ClientEvent::_collapseSlashes(std::string const &rawURI)
const {
	std::string	URI		= rawURI;
	::size_t	start	= URI.find_first_of('/');
	::size_t	end		= URI.find_first_not_of('/', start);

	while (start < URI.length())
	{
		if (end - start > 1)
			URI.replace(start, end - start, "/");
		start	= URI.find_first_of('/', start + 1);
		end		= URI.find_first_not_of('/', start);
	}

	if (URI.length() > 1 && URI.back() == '/')
		URI.pop_back();

	EasyPrint(rawURI);
	EasyPrint(URI);

	return (URI);
}

int
ClientEvent::_URIdentification()
{
	LocationMap	const	&locations = r_config.locations;

	std::string	const URI = _collapseSlashes(_request.getURI());

	if (locations.contains(URI)) {
		_target.location	= URI;
		_target.root		= locations.at(URI).root;
		_target.file		= "/";
	} else {
		::size_t const		lastSlash = URI.find_last_of('/');
		std::string			URIParent = URI.substr(0, lastSlash);

		if (URIParent.empty())
			URIParent = "/";

		if (!locations.contains(URIParent))
			return (-1);

		_target.location	= URIParent;
		_target.root		= locations.at(URIParent).root;
		_target.file		= URI.substr(lastSlash);

		::size_t const	dot	= _target.file.find('.');
		if (dot != std::string::npos)
			_target.extension	= _target.file.substr(dot);
	}

	EasyPrint(_target.root);
	EasyPrint(_target.file);

	return (0);
}

void
ClientEvent::_processRequest()
{
	std::string const	&method = _request.getMethod();

	if (!Methods.contains(method))
		throw HttpError(501);

	if (_URIdentification() == -1)
		throw HttpError(404);

	Config::Listener::Location const	&location = r_config.locations.at(_target.location);

	if (location.allowedMethods.find(method) == std::string::npos)
		throw HttpError(403);

	Methods.at(method)(location);
}

void
ClientEvent::_get(
	Config::Listener::Location const &location)
{
	if (_target.file == "/")
		_target.file = location.index;

	if (SupportedCGIExtensions.contains(_target.extension)) {
		_cgi(location);
		return;
	}

	std::string	path			= "." + _target.root + _target.file;
	std::string	indexContent	= IO::getFileContent(path);

	std::cout	<< "GET " << _request.getURI()
				<< " <" << path << "> "
				<< ((indexContent.empty()) ? "(empty)" : "") << "\n";

	if (indexContent.empty())
		throw HttpError(404);
	else
		_response.setEntityBody(indexContent, path);
}

void
ClientEvent::_post(
	Config::Listener::Location const &location)
{
	(void)location;

	std::string	path = "." + _target.root + location.uploadDir + _target.file;

	if (_target.file == "/") {
		for (::size_t i = 0; i < 100; ++i)
		{
			path = "." + _target.root + location.uploadDir + "/temp_file_" + std::to_string(i);
			if (!IO::exists(path))
				break;
		}
	}

	EasyPrint(path);
	std::ofstream	outfile(path);

	if (!outfile.is_open())
		throw HttpError(500);
	_response.setStatus(201);

	outfile << _request.getEntityBody();
	outfile.close();
}

void
ClientEvent::_delete(
	Config::Listener::Location const &location)
{
	(void)location;

	std::string		path = "." + _target.root + _target.file;

	if (_target.file == "/")
		throw HttpError(403);

	if (std::remove(path.c_str()) == -1)
		throw HttpError(500);
}

char **
ClientEvent::setupEnvironment(std::string const &scriptPath)
const {
	std::vector<std::string> envVars =
	{
		"REDIRECT_STATUS=200",
		"GATEWAY_INTERFACE=CGI/1.0",
		"PATH=/usr/bin:/bin",
		"REQUEST_METHOD=" + _request.getMethod(),
		"SERVER_PROTOCOL=" + _request.getVersion(),
		"SERVER_NAME=" + r_config.name,
		"SERVER_PORT=" + std::to_string(r_config.port),
		"CONTENT_TYPE=" + _request.getRequestHeaderValue("Content-Type"),
		"CONTENT_LENGTH=" + _request.getRequestHeaderValue("Content-Length"),
		"QUERY_STRING=" + _request.getQueryString(),
		"SCRIPT_NAME=" + _request.getScriptName(),
		"SCRIPT_FILENAME=" + scriptPath,
		"TZ=Europe/Amsterdam"
	};

	const Http::HeaderMap	&headers = _request.getRequestHeaders();
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
		envVars.push_back(cgiVarName + "=" + headerValue);
	}

	char**	envArray = new char*[envVars.size() + 1]{};
	for (uint32_t i = 0; i < envVars.size(); i++)
	{
		envArray[i] = new char[envVars[i].length() + 1];
		std::strcpy(envArray[i], envVars[i].c_str());
	}
	envArray[envVars.size()] = NULL;

	return (char**)envArray;
}

void
ClientEvent::_cgi(
	Config::Listener::Location const &location)
{
	if (location.cgiEXT.find(_target.extension) == std::string::npos)
		throw HttpError(403);

	int	pipe[2];
	if (::pipe(pipe) == -1)
		throw HttpError(500);

	_cgild = fork();
	EasyPrint(_cgild);
	if (_cgild == -1) {
		::close(pipe[0]);
		::close(pipe[1]);
		throw HttpError(500);
	}

	if (_cgild == 0) {
		EasyPrint(_target.file);
		::close(pipe[0]);
		::dup2(pipe[1], STDOUT_FILENO);
		::dup2(pipe[1], STDERR_FILENO);
		{
			std::string	interpreter	= SupportedCGIExtensions.at(_target.extension);
			std::string	path		= std::filesystem::absolute("." + _target.root + _target.file);

			if (chdir(std::string("." + _target.root).c_str()) < 0)
				exit(EXIT_FAILURE);

			char	**env	= setupEnvironment(path);
			char	*argv[]	= {
				(char *)interpreter.c_str(),
				(char *)path.c_str(),
				NULL
			};

			execve(argv[0], argv, env);
		}
		exit(errno);
	} else {
		::close(pipe[1]);

		EventHandlers::create<CGInboxEvent>(
			pipe[0], *this, r_epoll, r_config);

		std::cout << "CGInbox " << pipe[0] << " \e[33mCreated\e[0m\n";
	}
}

void
ClientEvent::parseMailHeaders(std::string const &headers)
{
	std::istringstream	headerStream(headers);

	std::string		headerLine;
	std::string		statusCode = "200";
	Http::HeaderMap	cgiHeaders;

	while(std::getline(headerStream, headerLine))
	{
		if (!headerLine.empty() && headerLine.back() == '\r')
			headerLine.pop_back();

		if (headerLine.empty())
			continue;

		::size_t colonPos = headerLine.find(':');
		if(colonPos == std::string::npos)
			continue;

		std::string key = headerLine.substr(0, colonPos);
		std::string value = headerLine.substr(colonPos + 1);

		::size_t firstNonSpace = value.find_first_not_of(" \t");
		if (firstNonSpace != std::string::npos)
			value = value.substr(firstNonSpace);

		if (key == "Status") {
			::size_t spacePos = value.find(' ');
			statusCode =	(spacePos != std::string::npos)
							? value.substr(0, spacePos)
							: value;
		} else if (key == "Location") {
			cgiHeaders["Location"] = value;
		} else if (key != "Content-Type" && key != "Content-Length") {
			cgiHeaders[key] = value;
		}
	}
}

void
ClientEvent::youHaveGotMail(std::string &cgiOutput)
{
	{	int		status;
		pid_t	pid = waitpid(_cgild, &status, WNOHANG);
		if (pid == 0)
			LOG(Warning, "cgi child: zombie process now");
		if (WIFEXITED(status) == false)
			LOG(Warning, "cgi child: did not exit");
		if (WIFEXITED(status) == true)
			LOG(Warning, "cgi child: exited with code " + std::to_string(WEXITSTATUS(status)));
		if (WIFSIGNALED(status) == true)
			LOG(Warning, "cgi child: signaled " + std::to_string(WTERMSIG(status)));
	}

	EasyPrint(cgiOutput);
	LOG(Info, cgiOutput);

	::size_t	headerEndPos = cgiOutput.find(HeaderEnd);
	if (headerEndPos != std::string::npos) {
		parseMailHeaders(cgiOutput.substr(0, headerEndPos));
		cgiOutput.erase(0, headerEndPos + HeaderEnd.length());
	}

	_response.setEntityBody(cgiOutput);

	_finalise();
}

ClientEvent::HttpError::HttpError(u_int16_t error)
	:	_status(500)
{
	if (error)
		_status = error;
	_what = std::to_string(_status) + " "
		+ (Http::Statuses.contains(_status)
		? Http::Statuses.at(_status)
		: "Unknown Status");
}

u_int16_t
ClientEvent::HttpError::status() const {
	return (_status);
}

const char *
ClientEvent::HttpError::what() const noexcept {
	return (_what.c_str());
}
