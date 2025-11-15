#ifndef EPOLL_HPP
# define EPOLL_HPP

// C
# include <unistd.h>
// C++
# include <stdexcept>
# include <vector>
// webserv
# include "Socket.hpp"
# include "ListenSocket.hpp"
# include "EasyThrow.hpp"
// wrappers
# include "WrapEpoll.hpp"

class	Epoll
{
	public:
		using	Ctl		= enum WrapEpoll::Ctl;

	private:
		static constexpr int	_EventBatchSize	= 64;
		static constexpr int	_waitTimeout	= 10000;

	public:
		Epoll();
		Epoll(Epoll const &other) = delete;
		Epoll(Epoll &&other) = delete;
		~Epoll();

		operator int() const;

		int					create() const;
		std::vector<epoll_event>	wait() const;
		int					ctl(Ctl operation, int fd, epoll_event *event = NULL) const;
		int					ctl(Epoll::Ctl operation, epoll_event &event) const;

	private:
		int	_fd;
};

#endif
