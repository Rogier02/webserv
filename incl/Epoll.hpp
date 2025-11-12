#ifndef EPOLL_HPP
# define EPOLL_HPP

# ifdef VERBOSE
#  include <iostream>
# endif

// C
# include <unistd.h>
# include <sys/epoll.h>
// C++
# include <stdexcept>
# include <vector>

class	Epoll
{
	public:
		enum	Event : std::uint32_t {
			In	= EPOLLIN,
			Out	= EPOLLOUT,
			Err	= EPOLLERR,
			Hup	= EPOLLHUP,
		};
		enum	Ctl : int {
			Add	= EPOLL_CTL_ADD,
			Del	= EPOLL_CTL_DEL,
		};

	private:
		static constexpr int	_EventBatchSize	= 64;
		static constexpr int	_waitTimeout	= 1000;

	public:
		Epoll(int socket);
		~Epoll();

		operator int() const;

		std::vector<epoll_event>	&wait();
		int							ctl(Ctl operation, int fd, epoll_event *event = std::nullptr) const;

	private:
		int	_fd;
		std::vector<epoll_event>	_events;
};

#endif
