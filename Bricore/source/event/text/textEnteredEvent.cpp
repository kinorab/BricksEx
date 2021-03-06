#include "textEnteredEvent.h"
#include "../eventListener.h"

namespace game {
	TextEnteredEvent::TextEnteredEvent(sf::Event::TextEvent eventData) :
		entered(eventData) {
	}

	void TextEnteredEvent::accept(EventListenerBase & visitor) {
        dynamic_cast<EventListener<TextEnteredEvent> &>(visitor).visit(*this);
	}
}
