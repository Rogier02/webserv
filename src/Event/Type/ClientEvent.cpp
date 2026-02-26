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
	LocationMap::const_iterator	locationEntry = _URIdentification();

	if (locationEntry == r_config.locations.end())
		return _response.err(404);

	Config::Listener::Location const	&location	= locationEntry->second;
	std::string const					&method		= _request.getMethod();

	if (location.allowedMethods.find(method) == std::string::npos)
		return _response.err(403);

	Methods[method](location);
}

ClientEvent::LocationMap::const_iterator
ClientEvent::_URIdentification()
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

	it = searchLocation(URI);

	if (it == r_config.locations.end()) {
		std::string	URILocation	= URI.substr(0, URI.find_last_of('/'));

		it = searchLocation(URILocation);
		if (it == r_config.locations.end())
			return (it);

		_resource.root	= it->second.root;
		_resource.file	= URI.substr(URI.find_last_of('/'));
	} else {
		_resource.root	= it->second.root;
		_resource.file	= "/";
	}

	EasyPrint(_resource.root);
	EasyPrint(_resource.file);

	return (it);
}

void
ClientEvent::_get(
	Config::Listener::Location const &location)
{
	if (_resource.file == "/")
		_resource.file = location.index;

	std::string	path			= "." + _resource.root + _resource.file;
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
	if (_resource.file == "/")
		return (_response.err(403));

	std::string		path = "." + _resource.root + _resource.file;
	std::ofstream	outfile(path);

	if (!outfile.is_open())
		return (_response.err(500));
	_response.setStatus(201);

	EasyPrint(_request.getEntityBody());
	EasyPrint(path);
	outfile << _request.getEntityBody();
	outfile.close();
}

void
ClientEvent::_delete(
	Config::Listener::Location const &location)
{
	if (_resource.file == "/")// probably needs more confirmation that root + file is a directory
		return (_response.err(403));

	std::string		path = "." + _resource.root + _resource.file;

	if (std::remove(path.c_str()) == -1)
		return (_response.err(500));
}
