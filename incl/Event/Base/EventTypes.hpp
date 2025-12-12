#ifndef EVENTTYPES_HPP
# define EVENTTYPES_HPP

// C++
# include <memory>
# include <map>
// webserv
# include "Event.hpp"

class	EventTypes
{
	public:
		EventTypes() = delete;

	private:
		static std::map<int, std::unique_ptr<Event>>	_handlers;

	public:
		// template <typename Type>
		// static void		specify(Type const &event);

		template <typename Type, typename... Args>
		static Type		&create(int fd, Args&&... args);
		static Event	*get(int fd);

	public:
		class	NotRegistered : public std::exception {
			public:
				const char *what() const throw() override;
		};
};

# include "EventTypes.tpp"

#endif
