#include "joystickPressedEvent.h"
#include "../eventListener.h"

namespace game {
	JoystickPressedEvent::JoystickPressedEvent(sf::Event::JoystickButtonEvent eventData) :
		sf::Event::JoystickButtonEvent(eventData) {
	}

	void JoystickPressedEvent::accept(EventListenerBase & visitor) {
        dynamic_cast<EventListener<JoystickPressedEvent> &>(visitor).visit(*this);
	}
}
