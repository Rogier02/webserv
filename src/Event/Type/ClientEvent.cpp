#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int socketFd, Epoll &epoll, Config::Listener const &config)
	:	Event(socketFd, Epoll::Events::In | Epoll::Events::RdH, epoll, config)
	,	_headersParsed(false)
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
		const std::size_t	headerEnd = _requestBuffer.find("\r\n\r\n") + 4;

		if (received == 0)
			return (EventHandlers::erase(data.fd));

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
				// std::cout << "\n\n" << _requestBuffer << "\n";
				std::cout << "\n\n" << _request.toString() << "\n";
				_processRequest();
		}
	}
	_responseBuffer = _response.toString();
	_mod(Epoll::Events::Out | Epoll::Events::RdH);
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
		std::size_t	const	lastSlash = URI.find_last_of('/');
		std::string const	URIParent = URI.substr(0, lastSlash);

		if (!locations.contains(URIParent))
			return (-1);

		_target.location	= URIParent;
		_target.root		= locations.at(URIParent).root;
		_target.file		= URI.substr(lastSlash);
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

void
ClientEvent::_cgi(
	Config::Listener::Location const &location)
{
	std::string	extension = _target.file.substr(_target.file.find('.'));

	if (location.cgiEXT.find(extension) == std::string::npos)
		return (_response.err(403));

	if (location.cgiPath.empty())
		return (_response.err(403));

	int		inPipe[2];
	int		outPipe[2];
	pid_t	pid;

	if (::pipe(inPipe) == -1)
		return (_response.err(500));
	if (::pipe(outPipe) == -1) {
		close(inPipe[0]);
		close(inPipe[1]);
		return (_response.err(500));
	}

	pid = fork();
	if (pid == -1) {
		close(inPipe[0]);
		close(inPipe[1]);
		close(outPipe[0]);
		close(outPipe[1]);
		return (_response.err(500));
	}

	if (pid == 0)
	{
		close(inPipe[1]);
		close(outPipe[0]);
		// cgiexec;
	}

	close(inPipe[0]);
	close(outPipe[1]);

	// create CGInbox event for reading from the outPipe
	// forward entityBody to inPipe
	// make yougotmail func to probably also waitpid()
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
		return (_response.err(403));// make temporary files instead

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
