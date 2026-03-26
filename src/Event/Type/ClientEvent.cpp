#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int socketFd, Epoll &epoll, Config::Listener const &config)
	:	Event(socketFd, Epoll::Events::In | Epoll::Events::RdH, epoll, config)
	,	_headersParsed(false)
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
	const ssize_t	received = Socket::recv(data.fd, _requestBuffer);

	if (_headersParsed == false) {
		if (received == -1 || _requestBuffer.empty())
			return (EventHandlers::erase(data.fd));

		const std::size_t headerEnd = _requestBuffer.find("\r\n\r\n") + 4;
		if (headerEnd == std::string::npos)
			return;

		if (_request.parseHead(_requestBuffer.substr(0, headerEnd)) == -1)
			_response.err(400);
		_requestBuffer.erase(0, headerEnd);
		_headersParsed = true;
	}
	if (_headersParsed == true) {
		switch (EasyPrint(_request.setEntityBody(_requestBuffer))) {
			case -1:
				_response.err(400);
				break;
			case 0:
				return;
			default:
				std::cout << "\n\n" << _request.toString() << "\n";
				_processRequest();
				if (_cgild > 0)// maybe make a 3-way state anyway, headers, entitybody, cgiwait...
					return;
		}
		_finalise();
	}
}

void
ClientEvent::_out()
{
	ssize_t	sent = Socket::send(data.fd, _responseBuffer);

	if (sent == -1) {
		if (!(errno == EAGAIN || errno == EWOULDBLOCK))
			EventHandlers::erase(data.fd);
		return;
	}

	_responseBuffer.erase(0, sent);

	if (_responseBuffer.empty()) {
		std::cout << "Client " << data.fd << " \e[32mCompleted Request.\e[0m\n";
		EventHandlers::erase(data.fd);
	}
}

void
ClientEvent::_finalise()
{
	_responseBuffer = _response.toString();
	_mod(Epoll::Events::Out | Epoll::Events::RdH);
}

std::string
ClientEvent::_collapseSlashes(std::string const &rawURI)
const {
	std::string	URI		= rawURI;
	std::size_t	start	= URI.find_first_of('/');
	std::size_t	end		= URI.find_first_not_of('/', start);

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
		std::size_t const	lastSlash = URI.find_last_of('/');
		std::string const	URIParent = URI.substr(0, lastSlash);

		if (!locations.contains(URIParent))
			return (-1);

		_target.location	= URIParent;
		_target.root		= locations.at(URIParent).root;
		_target.file		= URI.substr(lastSlash);

		std::size_t const	dot	= _target.file.find('.');
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
		return (_response.err(501));

	if (_URIdentification() == -1)
		return (_response.err(404));

	Config::Listener::Location const	&location = r_config.locations.at(_target.location);

	if (location.allowedMethods.find(method) == std::string::npos)
		return (_response.err(403));

	Methods.at(method)(location);
}

char **
ClientEvent::setupEnvironment()
{
	std::vector<std::string> _envVariables;

	_envVariables.push_back("REQUEST_METHOD=" + _request.getMethod());
	_envVariables.push_back("CONTENT_TYPE=" + _request.getRequestHeaderValue("Content-Type")); // TODO:: ROGIER :: doublecheck for this
	_envVariables.push_back("CONTENT_LENGTH=" + _request.getRequestHeaderValue("Content-Length"));
	_envVariables.push_back("SCRIPT_NAME=" + _request.getScriptName());
	_envVariables.push_back("QUERY_STRING=" + _request.getQueryString());
	_envVariables.push_back("REQUEST_URI=" + _request.getURI());
	_envVariables.push_back("SERVER_PROTOCOL=" + _request.getVersion());
	_envVariables.push_back("GATEWAY_INTERFACE=CGI/1.0"); // wordt dit geparsed uit de httpRequest? of is dit vast?
	_envVariables.push_back("SERVER_NAME=" + r_config.name);
	_envVariables.push_back("SERVER_PORT=" + std::to_string(r_config.port));
	_envVariables.push_back("SERVER_ADDR=" + r_config.host);

	// add HTTP headers as CGI variables

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
		_envVariables.push_back(cgiVarName + "=" + headerValue);
	}

	char **env = new char*[_envVariables.size() + 1];
	for (size_t i = 0; i < _envVariables.size(); i++) {
		env[i] = new char[_envVariables[i].length() + 1];
		std::strcpy(env[i], _envVariables[i].c_str());
	}
	env[_envVariables.size()] = NULL;

	return (env);
}

void
ClientEvent::_cgi(
	Config::Listener::Location const &location)
{
	if (location.cgiEXT.find(_target.extension) == std::string::npos)
		return (_response.err(403));

	if (CGI2::SupportedExtensions.contains(_target.extension) == false)
		return (_response.err(501));

	if (location.cgiPath.empty())
		return (_response.err(403));

	int	pipe[2];
	if (::pipe(pipe) == -1)
		return (_response.err(500));

	_cgild = fork();
	if (_cgild == -1) {
		::close(pipe[0]);
		::close(pipe[1]);
		return (_response.err(500));
	}

	if (_cgild == 0) {
		::close(pipe[0]);
		::dup2(pipe[1], STDOUT_FILENO);
		::dup2(pipe[1], STDERR_FILENO);
		{
			std::string	interpreter	= CGI2::SupportedExtensions.at(_target.extension);
			std::string	path		= "." + _target.root + _target.file;

			char	**env	= setupEnvironment();
			char	*argv[]	= {
				(char *)interpreter.c_str(),
				(char *)path.c_str(),
				NULL
			};

			execve(argv[0], argv, env);
		}
		exit(0);
	} else {
		::close(pipe[1]);

		EventHandlers::create<CGInboxEvent>(
			pipe[0], *this, r_epoll, r_config);

		std::cout << "CGInbox " << pipe[0] << " \e[33mSuccessfully Created.\e[0m\n";
	}
}

void
ClientEvent::_get(
	Config::Listener::Location const &location)
{
	if (_target.file == "/")
		_target.file = location.index;

	if (_target.file.ends_with(".py"))// obviously change this condition
		return (_cgi(location));

	std::string	path			= "." + _target.root + _target.file;
	std::string	indexContent	= IO::readFile(path);

	std::cout	<< "GET " << _request.getURI()
				<< " <" << path << "> "
				<< ((indexContent.empty()) ? "(empty)" : "") << "\n";

	if (indexContent.empty())
		_response.err(404);
	else
		_response.setEntityBody(indexContent);
}

void
ClientEvent::_post(
	Config::Listener::Location const &location)
{
	if (_target.file == "/")
		return (_response.err(403));// make temporary files instead?

	std::string		path = "." + _target.root + _target.file;
	// overwrite or append existing files?

	std::ofstream	outfile(path);

	if (!outfile.is_open())
		return (_response.err(500));
	_response.setStatus(201);

	outfile << _request.getEntityBody();
	outfile.close();
}

void
ClientEvent::_delete(
	Config::Listener::Location const &location)
{
	if (_target.file == "/")
		return (_response.err(403));

	std::string		path = "." + _target.root + _target.file;

	// forbid removing directories and maybe some other stuff

	if (std::remove(path.c_str()) == -1)
		return (_response.err(500));
}

void
ClientEvent::youHaveGotMail(std::string &CGIOutput)
{
	// parse CGI output

	EasyPrint(CGIOutput);

	_response.setEntityBody(CGIOutput);

	int	status;
	waitpid(_cgild, &status, 0);
	if (WIFEXITED(status) == false || WEXITSTATUS(status) != 0)
		_response.err(500);

	_finalise();
}
