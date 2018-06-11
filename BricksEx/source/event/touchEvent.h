#pragma once

#include "UIEvent.h"

namespace game {
	class TouchEvent :
		public sf::Event::TouchEvent,
		public UIEvent {
	public:
		TouchEvent(EventType type, sf::Event::TouchEvent eventData);
		virtual ~TouchEvent() = default;
		virtual void accept(EventListener & visitor) override;
	};
}