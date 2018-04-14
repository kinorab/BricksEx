#include "button.h"
#include "define.h"
#include "intersects.h"

namespace game {
	Button::Button()
		:text(new sf::Text()) {
		font.loadFromFile("arial.ttf");
		text->setFont(font);
		/*
		spriteUp.setTexture(up);
		spriteOver.setTexture(over);
		spriteDown.setTexture(down);
		currentSprite = &spriteUp;
		spriteUp.setPosition(location);
		spriteOver.setPosition(location);
		spriteDown.setPosition(location);

		setCaption(caption);
		text.setPosition(location.x + 3, location.y + 3);
		text.setCharacterSize(14);
		*/
		addChild({ text });
	}

	void Button::setCurrentState(ButtonState state) {
		currentState = state;
		if (currentState == ButtonState::UP) {
			currentSprite = &spriteUp;
		}
		else if (currentState == ButtonState::OVER) {
			currentSprite = &spriteOver;
		}
		else if (currentState == ButtonState::DOWN) {
			currentSprite = &spriteDown;
		}
	}

	void Button::setCaption(std::string caption) {
		text->setString(caption);
	}

	Button::ButtonState Button::getCurrentState() {
		return currentState;
	}

	std::string Button::getCaption() {
		return text->getString();
	}
}