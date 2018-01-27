#pragma once

#include "buttonInterface.h"
#include <memory>
#include <SFML/Graphics.hpp>

class UIFactory {
public:
	static std::unique_ptr<ButtonInterface> createButton(const sf::Texture & up, const sf::Texture & over, const sf::Texture & down, sf::FloatRect hitArea, std::string caption, sf::Vector2f location);
};