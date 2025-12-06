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
		using Fd = int;
		
}