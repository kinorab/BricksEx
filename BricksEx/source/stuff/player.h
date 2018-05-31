#pragma once

#include "../definition/diagonalPoint.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>

namespace sf {
	class Sound;
	class RenderTarget;
	class RenderStates;
	class Color;
}

class Player :
	public sf::Drawable
	, public sf::Transformable {

public:
	Player();
	explicit Player(const Player &copy);
	virtual void update(const sf::Vector2f &ballPos, const float ballRadius);
	virtual void preUpdate(const sf::Vector2f &ballPos, const float ballRadius, const float intervalTime);
	virtual const sf::Vector2f & getMainPlayerPos() const;
	virtual const sf::Vector2f getMainPlayerTopCenterPos() const;
	virtual const sf::FloatRect getMainPlayerBounds() const;
	virtual const sys::DPointf getMainPlayerDP() const;
	virtual Player & operator =(const Player &copy);
	virtual ~Player();

private:
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
	virtual void setFlashPosition(const sf::Vector2f &);
	virtual void setFlashPosition(const float, const float);
	virtual void setFlashFillColor(const sf::Color &);
	virtual void flashElapsed();
	virtual void flashRange(sf::Sound &, const sf::Vector2f, const float);

	bool flash;
	bool flashCD;
	sf::Clock CDTime;
	sf::Clock elapsed;
	sf::RectangleShape mainPlayer;
	sf::RectangleShape redRange;
	sf::RectangleShape yellowRange;
};
