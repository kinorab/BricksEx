#pragma once
#include "../definition/diagonalPoint.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <memory>
#include <vector>

namespace item {
	class Brick;
}

namespace sf {
	class Color;
}

namespace game {
	class Level;
}

class Ball;

class Wall :
	public sf::Drawable
	, public sf::Transformable {
public:
	explicit Wall(const std::shared_ptr<game::Level> level);
	void update(Ball &ball, const float updateRatio);
	void loadTexture(const std::string & fileName);
	void setBricksColor(const sf::Color & color);
	void setFramesColor(const sf::Color & color);
	void setBricksSize(const sf::Vector2f &sideLength);
	void setInterval(const sf::Vector2f &interval);
	void setFrameSize(const float frame);

	void resettle();

	size_t getBrickAmount() const;
	sys::DPointf getDP(const size_t number) const;
	float getFrameSize(const size_t number) const;
	const sf::Vector2f & getBrickSize(const size_t number) const;
	const sf::Color & getBrickColor(const size_t number) const;
	const sf::Vector2f & getInterval() const;
	virtual ~Wall();

protected:
	void settlePlace();

private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

	std::vector<std::shared_ptr<item::Brick>> bricks;
	// bricks on every row
	size_t uAmount;
	size_t uRowCount;
	float fFrameSize;
	bool bChangeEntity;
	sf::Vector2f sideLength;
	sf::Vector2f interval;
	sf::Vector2f whiteSpace;
	std::shared_ptr<game::Level> level;
};
