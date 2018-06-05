#include "mouseButtonEvent.h"

namespace game {
	MouseButtonEvent::MouseButtonEvent(EventType type, sf::Event::MouseButtonEvent eventData) :
		Event(type, true, true),
		sf::Event::MouseButtonEvent(eventData) {
	}

	void MouseButtonEvent::accept(EventListener * visitor) {
		visitor->visit(this);
	}
}
