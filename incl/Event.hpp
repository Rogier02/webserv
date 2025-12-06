#ifndef EPOLL_EVENT_HPP
# define EPOLL_EVENT_HPP

// C
# include <sys/epoll.h>

/**
 * @class Epoll::Event
 * @brief Wrapper around epoll_event with safe fd access
 * 
 * @note Cannot create reference to packed struct memebers
 * 			so we use methods to get and set fds
 */
class EpollEvent : public epoll_event
{
	public:
		/** @brief Type alias for file descriptor */
		using Fd = int;

	public:
		/**
		 * @brief Default constructor
		 * 
		 * Creates an uninitialized event
		 * 
		 * Typical use: For epoll.wait()
		 */
		EpollEvent() = default;
		
		/**
		 * @brief Constructor with file descriptor
		 * 
		 * Creates an event configured to monitor the given fd
		 * 
		 * @param socket File descriptor to monitor
		 * 
		 */
		explicit EpollEvent(int socket);

		/**
		 * @brief Copy constructor (defualt)
		 * 
		 * Safe to copy events
		 */
		EpollEvent(EpollEvent const& other) = default;

		/**
		 * @brief Move constructor (default)
		 * 
		 * Safe to move events (cheap operation)
		 */
		EpollEvent(EpollEvent&& other) = default;

		/**
		 * @brief Destructor default
		 * 
		 * No cleanup needed - just a struct wrapper
		 * Does NOT close the socket! You must do that.
		 */
		~EpollEvent() = default;

		public:
			operator int() const;

		public:
			/**
			 * @brief Check if event indicates error or disconnect
			 * 
			 * Returns true if:
			 * - EPOLLERR: Error on socket
			 * - EPOLLHUP: COnnection closed
			 * 
			 * When this is true, the socket should be closed
			 * 
			 * @return true if error or hangup event, false if normal data
			 * 
			 * @note this is the first check you should do on any event
			 */
			bool	isWeird() const;

			/**
			 * @brief Get file descriptor from this event
			 * 
			 * The file descriptor tells you WHICH socket had activity
			 * 
			 * @return File descriptor
			 * 
			 * @note Cannot use direct reference due to packed struct
			 * 		 That's why we use this method instead
			 */
			int		fd()const;

			/**
			 * @brief Set file descriptor for this event
			 * 
			 * Tell epoll which socket this event is for
			 * 
			 * @param fd_balue File descriptor to monitor
			 */
			void	setFd(int fd_value);

};

#endif