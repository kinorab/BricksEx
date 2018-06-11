#include "joystickConnectEvent.h"
#include "joystickConnectListener.h"

namespace game {
	JoystickConnectEvent::JoystickConnectEvent(EventType type, sf::Event::JoystickConnectEvent eventData) :
		UIEvent(type, false, false),
		sf::Event::JoystickConnectEvent(eventData) {
	}

	void JoystickConnectEvent::accept(EventListener & visitor) {
		JoystickConnectListener & listener = dynamic_cast<JoystickConnectListener &>(visitor);
		listener.visit(*this);
	}
}
