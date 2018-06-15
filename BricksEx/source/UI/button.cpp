#include "button.h"
#include "../definition/utility.h"
#include "../event/mouse/mouseButtonEvent.h"
#include "../event/mouse/mouseButtonListener.h"
#include "../event/mouse/mouseMovedListener.h"
#include "../event/mouse/mouseEnteredListener.h"
#include "../event/mouse/mouseLeftListener.h"
#include <SFML/Graphics.hpp>

namespace game {
	Button::Button(std::shared_ptr<sf::Sprite> upObject, std::shared_ptr<sf::Drawable> overObject, std::shared_ptr<sf::Drawable> downObject)
		: upObject(upObject)
		, overObject(overObject)
		, downObject(downObject)
		, hitObject(new SpriteNode(upObject)) {
		using namespace std::placeholders;
		addListener(std::make_shared<MouseEnteredListener>(std::bind(&Button::onMouseEntered, this, _1)));
		addListener(std::make_shared<MouseLeftListener>(std::bind(&Button::onMouseLeft, this, _1)));
		addListener(typeid(MouseButtonEvent::Pressed), std::make_shared<MouseButtonListener>(std::bind(&Button::onMousePressed, this, _1)));
		addListener(typeid(MouseButtonEvent::Released), std::make_shared<MouseButtonListener>(std::bind(&Button::onMouseReleased, this, _1)));
	}

	Button::~Button() {

	}

	bool Button::containsPoint(const sf::Vector2f & point) const {
		return hitObject->containsPoint(getTransform().getInverse().transformPoint(point));
	}

	std::shared_ptr<sf::Drawable> Button::getDrawable() const {
		switch (currentState) {
		case game::Button::ButtonState::UP:
			return upObject;
			break;
		case game::Button::ButtonState::OVER:
			return overObject;
			break;
		case game::Button::ButtonState::DOWN:
			return downObject;
			break;
		default:
			return nullptr;
			break;
		}
	}

	void Button::onMouseEntered(MouseEnteredEvent &) {
		currentState = ButtonState::OVER;
	}

	void Button::onMouseLeft(MouseLeftEvent &) {
		currentState = ButtonState::UP;
	}

	void Button::onMousePressed(MouseButtonEvent &) {
		currentState = ButtonState::DOWN;
	}

	void Button::onMouseReleased(MouseButtonEvent &) {
		currentState = ButtonState::OVER;
	}
}