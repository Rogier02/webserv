template <typename NewType, typename... Args>
NewType &
EventHandlers::create(int fd, Args&&... args)
{
	_handlers[fd] = std::make_unique<NewType>(fd, std::forward<Args>(args)...);
	return ((NewType &)*_handlers[fd]);
}
