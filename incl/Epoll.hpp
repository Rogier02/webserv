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
		using	Events	= enum WrapEpoll::Events;
		using	Ctl		= enum WrapEpoll::Ctl;

		// if Event somehow holds a Socket, it needs it's own files, for now it's just part of Epoll
		class	Event : public epoll_event {
			public:
				using	Fd		= int;

			public:
				Event() = default;
				Event(int socket);
				Event(Event const &other) = default;
				Event(Event &&other) = default;
				~Event() = default;

			public:
				operator int() const;

			public:
				bool	isWeird() const;

			public:
				Fd	&fd	= epoll_event::data.fd;
		};

	private:
		static constexpr Events	_defaultEvents	= Events::In;
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
