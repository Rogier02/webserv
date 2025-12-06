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

		/** @brief this is an operator  */
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
		 * @note Add: Start monitoring socket fd
		 * @note Del: Stop monitoring socket fd
		 * @note Mod: Change event to watch
		 * 
		 * @param operation What to do (Add, Del, Mod)
		 * @param fd File descriptor to operate on
		 * @param event Event object (required for Add/Mod, NULL for Del)
		 * 
		 * @return Result of operation (0 = Success, -1 = error)
		 * 
		 * @note Operation is thread-safe
		 * @note Can be called from any thread 
		 * @note Must match the Ctl::Enum for type safety
		 */
		int					ctl(Ctl operation, int fd, Event *event = NULL) const;

		/**
		 * @brief Control operations using Event objet directly
		 * 
		 * Convenience overload - extracts FD from Event object
		 * 
		 * @param operation What to do (Add, Del, Mod)
		 * @param event Event object containing fd and settings
		 */
		int					ctl(Epoll::Ctl operation, Event &event) const;

	private:
		/**
		 * @brief The epoll file descriptor
		 * 
		 * Created in constructor with epoll_create()
		 * Closed in destructor with close()
		 */
		int	_fd;
};

#endif
