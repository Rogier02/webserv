#ifndef WRAPEPOLL_HPP
# define WRAPEPOLL_HPP

// C
# include <sys/epoll.h>

namespace	WrapEpoll
{
	enum	Events : unsigned int {
		In	= EPOLL_EVENTS::EPOLLIN,
		Out	= EPOLL_EVENTS::EPOLLOUT,
		Err	= EPOLL_EVENTS::EPOLLERR,
		Hup	= EPOLL_EVENTS::EPOLLHUP,
	};

	enum	Ctl : int {
		Add	= EPOLL_CTL_ADD,
		Del	= EPOLL_CTL_DEL,
		Mod	= EPOLL_CTL_MOD,
	};
}

#endif
