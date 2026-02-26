#include "ClientEvent.hpp"

ClientEvent::ClientEvent(int fd, Config::Listener const &config)
	:	Event(fd, Epoll::Events::In | Epoll::Events::RdH)
	,	r_config(config)
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
	ssize_t	recieved = Socket::recv(data.fd, _requestBuffer);

	if (recieved == 0)
		_signal = Signal::Close;

	if (_requestBuffer.find("\r\n\r\n") == std::string::npos)
		return;

	// TODO: if content-length given, wait to recv whole request body
	std::cout << "\n\n" << _requestBuffer << "\n";

	if (_request.parse(std::move(_requestBuffer)) == -1) // leaves buffer empty
		_response.err(400);

	std::cout << "\n\n" << _request.toString() << "\n";

	// Process request, just don't block by waiting for IO like CGI
	_processRequest();

	_responseBuffer = _response.toString();
	_signal = Signal::Write;
}

void
ClientEvent::_out()
{
	ssize_t	sent = Socket::send(data.fd, _responseBuffer);

	if (sent == -1) {
		if (!(errno == EAGAIN || errno == EWOULDBLOCK))
			_signal = Signal::Close;
		return;
	}

	_responseBuffer.erase(0, sent);

	if (_responseBuffer.empty()) {
		std::cout << "Client " << data.fd << " \e[32mCompleted Request.\e[0m\n";
		_signal = Signal::Close;
	}
}

void
ClientEvent::_processRequest()
{
	std::string					resourcePath;
	LocationMap::const_iterator	locationEntry = _URIdentification(resourcePath);

	if (locationEntry == r_config.locations.end())
		return _response.err(404);

	Config::Listener::Location const	&location	= locationEntry->second;
	std::string const					&method		= _request.getMethod();

	if (location.allowedMethods.find(method) == std::string::npos)
		return _response.err(403);

	Methods[method](resourcePath, location);
}

ClientEvent::LocationMap::const_iterator
ClientEvent::_URIdentification(std::string &resourcePath)
{
	// replace all /////// with /
	std::function<LocationMap::const_iterator (std::string &path)>
		searchLocation = [this](std::string &path)->LocationMap::const_iterator {
			while (path.back() == '/')
				path.pop_back();
			EasyPrint(path);
			return (r_config.locations.find((path.empty()) ? "/" : path));
		};

	std::string	URI = _request.getURI();
	LocationMap::const_iterator	it;
	std::string	root;
	std::string	file;

	it = searchLocation(URI);

	if (it == r_config.locations.end()) {
		std::string	URILocation	= URI.substr(0, URI.find_last_of('/'));

		it = searchLocation(URILocation);
		if (it == r_config.locations.end())
			return (it);

		root	= it->second.root;
		file	= URI.substr(URI.find_last_of('/'));
	} else {
		root	= it->second.root;
		file	= it->second.index;// do this only in GET, leave empty here
	}

	EasyPrint(root);
	EasyPrint(file);

	resourcePath = "." + root + file;

	return (it);
}

void
ClientEvent::_get(
	std::string const &resourcePath,
	Config::Listener::Location const &location)
{
	std::string indexContent = IO::readFile(resourcePath);

	std::cout	<< "GET " << _request.getURI()
				<< " <" << resourcePath << "> "
				<< ((indexContent.empty()) ? "(empty)" : "") << "\n";

	if (indexContent.empty())
		_response.err(404);
	else
		_response.setEntityBody(indexContent);

}

void
ClientEvent::_post(
	std::string const &resourcePath,
	Config::Listener::Location const &location)
{
	if (resourcePath == "." + location.root + location.index)
		return (_response.err(403));

	std::ofstream	outfile(resourcePath);

	if (!outfile.is_open())
		return (_response.err(500));
	_response.setStatus(201);

	EasyPrint(_request.getEntityBody());
	EasyPrint(resourcePath);
	outfile << _request.getEntityBody();
	outfile.close();
}

void
ClientEvent::_delete(
	std::string const &resourcePath,
	Config::Listener::Location const &location)
{
	struct stat	pathInfo;

	stat(resourcePath.c_str(), &pathInfo);
	if (S_ISDIR(pathInfo.st_mode))
		return (_response.err(403));

	if (std::remove(resourcePath.c_str()) == -1)
		return (_response.err(500));
}
