#pragma once

#include "../common.h"
#include <map>
#include <functional>

namespace game {
	class Event;
	class EventListenerBase;

	class EventSubject {
	public:
		virtual ~EventSubject() = default;
		virtual int addListener(std::shared_ptr<EventListenerBase> listener);
		virtual void dispatchEvent(Event & event);
		virtual void removeListener(std::type_index eventType, int id);
	protected:
		EventSubject() = default;
		std::multimap<const std::type_index, std::pair<const int, std::shared_ptr<EventListenerBase>>> listeners;
		int idCount = 0;
	};
}
