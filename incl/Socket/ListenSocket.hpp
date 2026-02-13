#ifndef LISTENSOCKET_HPP
# define LISTENSOCKET_HPP

// webserv
# include "Socket.hpp"
// wrappers
# include "WrapIn.hpp"

class	ListenSocket : public Socket
{
	private:
		using	SocketType				= WrapIn::SocketType;
		using	SocketOptionLevel		= WrapIn::SocketOptionLevel;
		using	SocketOption			= WrapIn::SocketOption;
		using	SocketAddress			= WrapIn::SocketAddress;
		using	InternetAddress			= WrapIn::InternetAddress;
		using	AddressFamily			= WrapIn::AddressFamily;
		using	InternetSocketAddress	= WrapIn::InternetSocketAddress;

	public:
		ListenSocket() = delete;
		ListenSocket(ListenSocket const &) = delete;
		ListenSocket(ListenSocket &&) = delete;
		ListenSocket(int port);
		~ListenSocket() = default;
};

#endif
