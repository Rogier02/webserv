#include "Event.hpp"

Event::Event(int fd, u_int32_t eventTypes, Epoll &epoll, Config::Listener const &config)
	:	r_epoll(epoll)
	,	r_config(config)
{
	data.fd = fd;
	events = eventTypes;

	r_epoll.ctl(Epoll::Ctl::Add, *this);
}

Event::~Event()
{
	r_epoll.ctl(Epoll::Ctl::Del, data.fd);
	close(data.fd);
}

void
Event::handle()
{
	if (events & Epoll::Events::In)
		_in();
	if (events & Epoll::Events::Out)
		_out();
}

void	Event::_in() {}

void	Event::_out() {}

void
Event::_mod(u_int32_t eventTypes)
{
	events	= eventTypes;

	EasyThrow(r_epoll.ctl(Epoll::Ctl::Mod, *this));
}
