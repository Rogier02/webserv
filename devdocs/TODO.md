CGI error handling

ClientEvent Request Processing
	GET, POST, DELETE methods
	headers.....

CGI (parent) Event with pipe read fd
	sends output directly to ClientEvent so it can build the response and start sending

Timeout clients

Receive entity body if content length > 0

new Index page

Laura:	schrijf eigen getline met delim: CRLF
		validate headers if nodig
		check of in request line maar 1 SP staat tussen tokens
		if no root dan location root is algemene root, else gebruik location root
		validation config checken of gevalidate moet worden of map bestaat etc etc
		config parser checken of alles werkt (eind project)
