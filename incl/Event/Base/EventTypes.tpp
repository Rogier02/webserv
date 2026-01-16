// template <typename Type>
// void EventTypes::specify(Type const &event)
// {
// 	_handlers[event.data.fd] = std::make_unique<Type>(event);
// }

template <typename NewType, typename... Args>
NewType &
EventTypes::create(int fd, Args&&... args)
{
	_handlers[fd] = std::make_unique<NewType>(fd, std::forward<Args>(args)...);
	return ((NewType &)*_handlers[fd]);
}
