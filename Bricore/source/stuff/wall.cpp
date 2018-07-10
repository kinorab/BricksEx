#include "wall.h"
#include "ball.h"
#include "component/globular.h"
#include "component/mainBall.h"
#include "component/brick.h"
#include "../definition/gameState.h"
#include "../definition/utility.h"
#include "../gameSys/level/level.h"
#include "../gameSys/level/area/zone.h"
#include "../event/SFMLMouseHandler.h"
#include "../event/SFMLKeyboardHandler.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace item;

Wall::Wall(const std::shared_ptr<game::Level> level)
	: whiteSpace(0.0f, 0.0f)
	, bChangeEntity(false)
	, m_level(std::move(level)) {
	resettle();
}

void Wall::update(const float updateRatio) {
	if (bricks.empty()) {
		m_level->finishLevel();
		resettle();
		return;
	}
	for (size_t i = 0; i < getBrickAmount(); ++i) {
		for (auto element : m_ball->enteredWallArea()) {
			if (element->isCollidedWall(bricks.at(i).get())) {
				if (std::dynamic_pointer_cast<MainBall>(element)) {
					m_ball->ballDivided(10);
				}
				removeChild({ bricks.at(i) });
				bricks.erase(bricks.begin() + i);
				--i;
				break;
			}
		}
	}
}

void Wall::handle(const sf::Event & event) {
	mouseHandler->handle(event, *this, false);
	keyboardHandler->handle(event, *this);
}

void Wall::resetCopyTarget(const std::shared_ptr<Ball> ball) {
	m_ball = std::move(ball);
}

void Wall::loadTexture(const std::string & fileName) {
	for (size_t i = 0; i < getBrickAmount(); ++i) {
		bricks.at(i)->loadTexture(fileName);
	}
}

void Wall::setBricksColor(const Color &color) {
	for (size_t i = 0; i < getBrickAmount(); ++i) {
		bricks.at(i)->setBrickColor(color);
	}
}

void Wall::setFramesColor(const Color &color) {
	for (size_t i = 0; i < getBrickAmount(); ++i) {
		bricks.at(i)->setFrameColor(color);
	}
}

void Wall::setBricksSize(const Vector2f & sideLength) {
	if (sideLength.x <= 0 || sideLength.y <= 0) throw std::invalid_argument("Side-length cannot be negative.");
	this->sideLength = sideLength;
	bChangeEntity = true;
	settlePlace();
}

void Wall::setInterval(const Vector2f &interval) {
	if (interval.x < 0.0f || interval.y < 0.0f) throw std::invalid_argument("Interval cannot be negative.");
	this->interval = interval;
	settlePlace();
}

void Wall::setFrameSize(const float frameSize) {
	if (frameSize < 0.0f) throw std::invalid_argument("Frame size cannot be negative.");
	this->fFrameSize = frameSize;
	bChangeEntity = true;
	settlePlace();
}

void Wall::resettle() {
	const size_t rowCount = static_cast<size_t>(m_level->deploy->getBrick().it.at(0));
	const float wid = m_level->deploy->getBrick().it.at(1);
	const float hei = m_level->deploy->getBrick().it.at(2);
	const Vector2f &interval = sf::Vector2f(m_level->deploy->getBrick().it.at(3)
		, m_level->deploy->getBrick().it.at(4));
	const float frameSize = m_level->deploy->getBrick().it.at(5);
	const float whiteSpaceY = m_level->deploy->getBrick().it.at(6);

	if (wid <= 0.0f || hei <= 0.0f
		|| interval.x < 0.0f || interval.y < 0.0f
		|| frameSize < 0.0f || whiteSpaceY < 0.0f) {
		throw std::invalid_argument("Invaild brick initialization.");
	}
	sideLength = Vector2f(wid, hei);
	this->uRowCount = rowCount;
	this->fFrameSize = frameSize;
	this->interval = interval;
	uAmount = static_cast<size_t>((LEVEL_WIDTH - getInterval().x) / (getInterval().x + sideLength.x + this->fFrameSize * 2));
	whiteSpace.x = (LEVEL_WIDTH - ((this->interval.x + sideLength.x + this->fFrameSize * 2) * uAmount - this->interval.x)) / 2;
	whiteSpace.y = whiteSpaceY;
	bricks.resize(uRowCount * uAmount);

	if (getBrickAmount() != uRowCount * uAmount) {
		throw std::out_of_range("The subscripts are out of range.");
	}
	bChangeEntity = true;
	removeAllChildren();
	settlePlace();
	setBricksColor(m_level->deploy->getBrick().color.at(0));
}

size_t Wall::getBrickAmount() const {
	return bricks.size();
}

const Vector2f & Wall::getBrickSize(const size_t number) const {
	return bricks.at(number)->getBrickSize();
}

const Vector2f & Wall::getInterval() const {
	return interval;
}

float Wall::getFrameSize(const size_t number) const {
	return bricks.at(number)->getFrameSize();
}

sys::DPointf Wall::getDP(const size_t number) const {
	return bricks.at(number)->getDP();
}

const Color & Wall::getBrickColor(const size_t number) const {
	return bricks.at(number)->getBrickColor();
}

Wall::~Wall() { 
	removeAllChildren();
}

void Wall::settlePlace() {
	const Vector2f initialPos(whiteSpace.x + sideLength.x / 2 + fFrameSize, interval.y + fFrameSize + sideLength.y / 2);
	Vector2f tempPos = Vector2f(initialPos.x, initialPos.y + whiteSpace.y);
	const float height = uRowCount * (sideLength.y + fFrameSize * 2 + interval.y) + interval.y + whiteSpace.y;
	if (bChangeEntity) {
		std::for_each(bricks.begin(), bricks.end(), [this](std::shared_ptr<Brick> &element) {
			element.reset(new Brick(sideLength, fFrameSize));
			addChild({ element });
			if (fFrameSize > 0.0f) {
				element->setFrameColor(Color::Black);
			}
		});
		using namespace game;
		Zone::getInstance().settleZone(Zone::Wall, height);
		bChangeEntity = false;
	}
	// ensure that total with the intervals should not be out of screen
	if (whiteSpace.x < 0.0f) throw std::domain_error("Intervals are too large to place.");
	// start placing bricks array
	size_t tempCount = 1;
	std::for_each(bricks.begin(), bricks.end(), [&](const std::shared_ptr<Brick> &element) {
		const FloatRect bounds = element->getGlobalBounds();
		element->setPosition(tempPos);
		if (tempCount < uAmount) {
			tempPos += Vector2f(interval.x + bounds.width, 0);
			++tempCount;
		}
		else {
			tempPos = Vector2f(initialPos.x, tempPos.y + bounds.height + interval.y);
			tempCount = 1;
		}
	});
}

void Wall::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();
	Container::draw(target, states);
}