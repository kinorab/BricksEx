#include "core.h"

using namespace item;

Core::Core(const Kind core, const std::shared_ptr<sf::Texture> & texture)
	: it(core)
	, context(new sf::Sprite(*texture)) {
}

bool Core::containsPoint(const sf::Vector2f & point) const {
	return context->getGlobalBounds().contains(point);
}

std::shared_ptr<sf::Drawable> Core::getDrawable() const {
	return std::const_pointer_cast<sf::Drawable>(std::static_pointer_cast<const sf::Drawable>(shared_from_this()));
}

void Core::setOrigin(const sf::Vector2f & origin) {
	context->setOrigin(origin);
}

void Core::setOrigin(const float x, const float y) {
	context->setOrigin(x, y);
}

void Core::setPosition(const sf::Vector2f & position) {
	context->setPosition(position);
}

void Core::setPosition(const float x, const float y) {
	context->setPosition(x, y);
}

const sf::Vector2f & Core::getPosition() const {
	return context->getPosition();
}

sf::FloatRect Core::getLocalBounds() const {
	return context->getLocalBounds();
}

sf::FloatRect Core::getGlobalBounds() const {
	return context->getGlobalBounds();
}

Core::~Core() {
}