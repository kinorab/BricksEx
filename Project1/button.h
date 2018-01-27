#pragma once

#include "buttonInterface.h"

enum class ButtonState {
	UP,
	OVER,
	DOWN
};

class Button : public ButtonInterface {
public:
	Button(const sf::Texture & up, const sf::Texture & over, const sf::Texture & down, sf::FloatRect hitArea, std::string caption, sf::Vector2f location);
	void checkClick(sf::Vector2f mousePos);
	void setCaption(std::string caption);
	std::string getCaption();

private:
	sf::Sprite spriteUp;
	sf::Sprite spriteOver;
	sf::Sprite spriteDown;
	sf::FloatRect hitArea;
	sf::Sprite * currentSprite;
	sf::Text text;
	ButtonState currentState;
	void setCurrentState(ButtonState state);
	ButtonState getCurrentState();
};

ButtonInterface * ButtonInterface::create(const sf::Texture & up, const sf::Texture & over, const sf::Texture & down, sf::FloatRect hitArea, std::string caption, sf::Vector2f location) {
	return new Button(up, over, down, hitArea, caption, location);
}
