#ifndef EPOLL_HPP
# define EPOLL_HPP

// C
# include <sys/epoll.h>
# include <sys/resource.h>
# include <unistd.h>
// C++
# include <stdexcept>
# include <vector>
// webserv
# include "Socket.hpp"
# include "ListenSocket.hpp"
# include "EasyThrow.hpp"
# include "EasyPrint.hpp"

class	Epoll
{
	public:
		enum	Ctl : int {
			Add	= EPOLL_CTL_ADD,
			Del	= EPOLL_CTL_DEL,
			Mod	= EPOLL_CTL_MOD,
		};
		enum	Events : uint32_t {
			In	= EPOLL_EVENTS::EPOLLIN,
			Out	= EPOLL_EVENTS::EPOLLOUT,
			Err	= EPOLL_EVENTS::EPOLLERR,
			Hup	= EPOLL_EVENTS::EPOLLHUP,
		};

	public:
		Epoll();
		Epoll(Epoll const &other) = delete;
		Epoll(Epoll &&other) = delete;
		~Epoll();

	public:
		std::vector<epoll_event>	wait(int timeout_ms);

		int	ctl(Ctl operation, int fd, epoll_event *event = NULL);
		int	ctl(Epoll::Ctl operation, epoll_event &event);

	private:
		int				_epfd;
		unsigned long	_bufferSize;
};

#endif
