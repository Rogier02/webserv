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
# include "Event.hpp"
// wrappers
# include "WrapEpoll.hpp"

class	Epoll
{
	public:
		using	Ctl		= enum WrapEpoll::Ctl;

	private:
		static constexpr int	_EventBatchSize	= 64;
		static constexpr int	_waitTimeout	= 1000;

	public:
		Epoll();
		Epoll(ListenSocket const &listenSocket);
		Epoll(Epoll const &other) = delete;
		Epoll(Epoll &&other) = delete;
		~Epoll();

		operator int() const;

		int					create() const;
		std::vector<Event>	wait() const;
		int					ctl(Ctl operation, int fd, Event *event = NULL) const;
		int					ctl(Epoll::Ctl operation, Event &event) const;

	private:
		int	_fd;
};

#endif
