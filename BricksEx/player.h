#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "diagonalPoint.h"

class Player : public sf::Drawable {

public:
	static std::shared_ptr<Player> getInstance();
	static bool resetInstance();
	static void playerMove(sf::Sound &sound, const sf::Vector2f ballPos, const float radius);

	static const sf::Vector2f & getMainPlayerPos();
	static const sf::Vector2f getMainPlayerTopCenterPos();
	static const sf::FloatRect getMainPlayerBounds();
	static const sys::DPointf getMainPlayerDP();
	static const sys::DPointf getPlayerAreaDP();
protected:
	Player();

private:
	static std::shared_ptr<Player> instance;
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
	static void setFlashPosition(const sf::Vector2f &position);
	static void setFlashPosition(const float posX, const float posY);
	static void setFlashFillColor(const sf::Color &color);
	static void flashElapsed();
	static void flashRange(sf::Sound &sound, const sf::Vector2f ballPos, const float radius);

	static bool flash;
	static sf::Clock elapsed;
	static sf::RectangleShape mainPlayer;
	//static sf::RectangleShape subPlayer;
	static sf::RectangleShape redRange;
	static sf::RectangleShape yellowRange;
	static sf::RectangleShape playerArea;
};

