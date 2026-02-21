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
		enum	Events : u_int32_t {
			In	= EPOLL_EVENTS::EPOLLIN,
			Out	= EPOLL_EVENTS::EPOLLOUT,
			Err	= EPOLL_EVENTS::EPOLLERR,
			Hup	= EPOLL_EVENTS::EPOLLHUP,
			RdH	= EPOLL_EVENTS::EPOLLRDHUP,
		};

	public:
		Epoll();
		Epoll(Epoll const &) = delete;
		Epoll(Epoll &&) = delete;
		~Epoll();

	public:
		std::vector<epoll_event>	wait(int timeout_ms);

		int	ctl(Ctl operation, int fd, epoll_event *event = NULL);
		int	ctl(Ctl operation, epoll_event &event);

	private:
		int				_epfd;
		epoll_event		*_buffer;
};

#endif
