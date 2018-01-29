#pragma once

#include <SFML/Graphics.hpp>

class Block : public sf::VertexArray {
public:

	Block(const sf::Vector2f &position, float width, float height);
	void setVerticeColor(const sf::Color &);
	void setVerticeColor(const sf::Color &, const sf::Color &, const sf::Color &, const sf::Color &);
	void setWidth(const float width);
	void setHeight(const float height);
	void resetPosition();
	void setSpeed(const float ballSpeedX, const float ballSpeedY = 0.0f);
	void setSpeed(const sf::Vector2f &speed);
	void enable(sf::CircleShape &ball, float &ballSpeedX, float &ballSpeedY);
	void move();

	const sf::Vector2f & getCurrentPosition() const;
	const sf::Vector2f & getOriginPosition() const;
	const sf::Vector2f & getSpeed() const;
	const float getWidth() const;
	const float getHeight() const;

private:
	void setBlockVertice(const sf::Vector2f &, const float, const float);
	void blockCollision(std::vector<Block*> block);
	void moveEntity(const sf::Vector2f &);

	sf::Vector2f position;
	sf::Vector2f oriPos;
	sf::Vector2f speed;
	//std::vector <Block*> block;
	float width;
	float height;
};