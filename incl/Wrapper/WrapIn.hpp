#ifndef WIN_HPP
# define WIN_HPP

// C
# include <netinet/in.h>

namespace WrapIn
{
	enum	SocketType {
		Stream	= SOCK_STREAM,
	};

	enum	SocketOptionLevel {
		Socket	= SOL_SOCKET,
	};

	enum	SocketOption {
		ReuseAddress	= SO_REUSEADDR,
	};

	using	SocketAddress	= sockaddr;

	enum	InternetAddress {
		Any	= INADDR_ANY,
	};

	enum	AddressFamily {
		Inet	= AF_INET,
	};

	struct	InternetSocketAddress : private sockaddr_in {
		private:
			using	Family	= sa_family_t;
			using	Port	= in_port_t;
			using	Address	= in_addr_t;
		public:
			Family	&family	= sockaddr_in::sin_family;
			Port	&port	= sockaddr_in::sin_port;
			Address	&address= sockaddr_in::sin_addr.s_addr;
	};
}

#endif
