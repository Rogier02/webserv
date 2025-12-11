#ifndef EVENTTYPES_HPP
# define EVENTTYPES_HPP

// C++
# include <memory>
# include <vector>
# include <map>
// webserv
# include "Event.hpp"

class	EventTypes
{
	public:
		EventTypes() = delete;
		EventTypes(EventTypes const &other) = delete;
		EventTypes &operator=(EventTypes const &other) = delete;
		~EventTypes() = default;

	private:
		static std::vector<std::unique_ptr<Event>>		_prototypes;
		static std::map<int, std::unique_ptr<Event>>	_handlers;

	public:
		static void		add(std::unique_ptr<Event> prototype);
		static Event	*get(int fd);

		template <typename Type>
		static void		specify(Event const &event);

	public:
		class	NotRegistered : public std::exception
		{
			public:
				const char *what() const throw() override;
		};
};

# include "EventTypes.tpp"

#endif
