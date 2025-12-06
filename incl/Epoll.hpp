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

/**
 * @class Epoll
 * @brief Linux epoll wrapper for efficient I/O multiplexing
 * 
 * @note **Events**: What activity happened on a socket?
 * 		 - EPOLLIN: Socket has data to read
 * 		 - EPOLLOUT: Socket is ready to write
 * 		 - EPOLLERR: Error occurred
 * 		 - EPOLLHUP: Connection closed
 * 
 * @note **Monitoring**: Epoll watches for specific events on specific FDs
 * 		 epoll.ctl(Add, fd) -> Start watching FD
 * 		 epoll.ctl(Del, fd) -> Stop watching FD
 * 		 epoll.ctl(Mod, fd) -> Change which events to watch
 */
class	Epoll
{
	public:
		using	Events	= enum WrapEpoll::Events;
		using	Ctl		= enum WrapEpoll::Ctl;
		using	Event	= EpollEvent;

	private:
		/**
		 * @brief Default events to monitor
		 * 
		 * EPOLLIN = Socket has data to read
		 * We want to know when clients send requets
		 */
		static constexpr Events	_defaultEvents	= Events::In;

		/**
		 * @brief Maximum events returned per epoll.wait() call
		 * 
		 * @note When epoll.wait() returns, it gives up to 64 events
		 * Process 64 events per loop. 
		 * Over 64 events means they will be processed on the next pass
		 */
		static constexpr int	_EventBatchSize	= 64;

		/**
		 * @brief Timeout for epoll.wait() in milliseconds
		 * 
		 * -1   = Block indefinitely until activity
		 * 0    = Dont block, return immediately
		 * 1000 = Wait max 1 second before returning
		 * 
		 * Current 1000ms can be adjusted
		 */
		static constexpr int	_waitTimeout	= 1000;

	public:
		Epoll();
		Epoll(ListenSocket const &listenSocket);
		Epoll(Epoll const &other) = delete;
		Epoll(Epoll &&other) = delete;
		~Epoll();

		operator int() const;

		/**
		 * @brief Create a new epoll instance
		 * 
		 * @return File descriptor for the epoll instance
		 * 
		 * @note is called by the constructor
		 */
		int					create() const;

		/**
		 * @brief Wait for I/O activity on monitored sockets
		 * 
		 * @return Vector of Events representing active sockets
		 * 
		 * @example
		 * std::vectore<Epoll::Event> events = epoll.wait();
		 * // epoll.wait() returned, something happend!
		 * 
		 * for (auto& event : events) {
		 * 		std::cout << "activitiy on Fd: " << event.fd() << std::endl;
		 * }
		 * 
		 * @note Returned vectore only contains active events
		 * @note Returns immediately when ANY socket has activity
		 * @note Returns after timeout (1sec) even if nothing happend
		 * @note This is the BLOCKING CALL = where the server idles
		 *
		 */
		std::vector<Event>	wait() const;

		/**
		 * @brief Control operations (Add, Del, Mod)
		 * 
		 * 
		 */
		int					ctl(Ctl operation, int fd, Event *event = NULL) const;
		int					ctl(Epoll::Ctl operation, Event &event) const;

	private:
		int	_fd;
};

#endif
