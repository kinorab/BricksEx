#include "globular.h"
#include "../../definition/gameState.h"
#include "../../definition/intersects.h"
#include "../../gameSys/area/area.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace item;

void Globular::setColor(const Color &color) {
	ball->setFillColor(color);
}

void Globular::restartClock() {
	countTime.restart();
}

bool Globular::isEnteredObstacleArea() const {
	const float radius = getRadius();
	const Vector2f ballPos = getPosition();
	auto &obstacleArea = game::Area::getInstance().getArea(game::Area::Obstacle);
	return sys::ballRectINCIntersects(ballPos, radius, obstacleArea.getGlobalBounds());
}

bool Globular::isEnteredWallArea() const {
	const float radius = getRadius();
	const Vector2f ballPos = getPosition();
	auto &wallArea = game::Area::getInstance().getArea(game::Area::Wall);
	return sys::ballRectINCIntersects(ballPos, radius, wallArea.getGlobalBounds());
}

Globular::Globular()
	: hitside{ false, false, false, false }
	, bBroke(false)
	, bCD(false)
	, bActive(false)
	, ball(new CircleShape) {
	ball->setFillColor(Color::White);
	ball->setOutlineColor(Color::Black);
	ball->setRadius(5.f);
	ball->setOrigin(Vector2f(ball->getRadius(), ball->getRadius()));
	ball->setOutlineThickness(2.f);
}

void Globular::setActive(const bool active) {
	bActive = active;
}

void Globular::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(*ball, states);
}

void Globular::move(const sys::DPointf &DP, const float playerSpeed, const float updateRatio) {
	const Vector2f ballPos(getPosition());
	const float radius = getRadius();
	if (!bActive) {
		oriSpeed = ballSpeed;
		countTime.restart();
		bActive = true;
	}
	else if (countTime.getElapsedTime().asSeconds() > RESETTIME) {
		// preserve last speed then add 60% extra origin speed to new speed
		const sf::Vector2f temp(ballSpeed);
		resetBall();
		oriSpeed = ballSpeed;
		ballSpeed.x >= 0 ? ballSpeed.x += (abs(temp.x) - ballSpeed.x) * .6f
			: ballSpeed.x += -(abs(temp.x) + ballSpeed.x) * .6f;
		ballSpeed.y >= 0 ? ballSpeed.y += (abs(temp.y) - ballSpeed.y) * .6f
			: ballSpeed.y += -(abs(temp.y) + ballSpeed.y) * .6f;
		countTime.restart();
	}

	// out of bottom bound, reset the mainBall
	if (ballPos.y - radius > LEVEL_HEIGHT) {
		bBroke = true;
	}
	// window's right bound
	else if (ballPos.x + radius >= LEVEL_WIDTH) {
		ballSpeed.x = -abs(ballSpeed.x);
	}
	// window's left bound
	else if (ballPos.x - radius <= 0) {
		ballSpeed.x = abs(ballSpeed.x);
	}
	// window's top bound
	else if (ballPos.y - radius <= 0) {
		ballSpeed.y = abs(ballSpeed.y);
	}
	// the collision between mainBall and player
	else if (sys::ballRectINCIntersects(ballPos, radius, DP)) {
		if (!bCD) {
			countTime.restart();
			if (ballSpeed.x == 0.0f) {
				ballSpeed.x = oriSpeed.x;
			}
			hitByPlayer(DP, playerSpeed, false);
			bCD = true;
			CDTime.restart();
		}
		else if (CDTime.getElapsedTime().asSeconds() > 0.1f) {
			hitByPlayer(DP, playerSpeed, true);
			bCD = false;
		}
		// prevent speed too fast
		if (abs(ballSpeed.x) >= MAXSPEED) {
			if (ballSpeed.x < 0) {
				ballSpeed.x = -MAXSPEED;
			}
			else {
				ballSpeed.x = MAXSPEED;
			}
		}
	}
	determineHitDirect();
	ball->move(ballSpeed / static_cast<float>(SLICE) * updateRatio);
}

void Globular::hitByPlayer(const sys::DPointf &DP, const float playerSpeed, const bool continuousCollision) {
	const Vector2f ballPos(getPosition());
	const Vector2f LT(DP.dot1);
	const Vector2f RB(DP.dot2);
	const float width = RB.x - LT.x;
	ballSpeed.y = -abs(ballSpeed.y);
	if (continuousCollision) {
		if (ballPos.x > LT.x + width / 2) {
			ballSpeed.x = abs(ballSpeed.x);
		}
		else {
			ballSpeed.x = -abs(ballSpeed.x);
		}
		return;
	}
	// hit on the player board
	if (ballPos.x <= RB.x && ballPos.x >= LT.x) {
		// right side of player position
		if (ballPos.x > LT.x + width * .9f) {
			ballSpeed.x = abs(ballSpeed.x) * 1.125f;
		}
		else if (ballPos.x > LT.x + width * .75f) {
			ballSpeed.x = abs(ballSpeed.x) * 1.075f;
		}
		else if (ballPos.x > LT.x + width * .55f) {
			ballSpeed.x = abs(ballSpeed.x) * 1.05f;
		}
		// left side of player position
		else if (ballPos.x < LT.x + width * .1f) {
			ballSpeed.x = -abs(ballSpeed.x) * 1.125f;
		}
		else if (ballPos.x < LT.x + width * .25f) {
			ballSpeed.x = -abs(ballSpeed.x) * 1.075f;
		}
		else if (ballPos.x < LT.x + width * .45f) {
			ballSpeed.x = -abs(ballSpeed.x) * 1.05f;
		}
		// hit center range(0.45f ~ 0.55f) will reset all speed, ballSpeed.x will not change direct
		// hit at center position
		else if (ballPos.x == LT.x + width / 2) {
			ballSpeed.x = 0.0f;
			ballSpeed.y = -abs(oriSpeed.y);
		}
		else {
			ballSpeed.x < 0 ? ballSpeed.x = -abs(oriSpeed.x) : ballSpeed.x = abs(oriSpeed.x);
			ballSpeed.y = -abs(oriSpeed.y);
		}
	}
	// hit at the player edge
	else {
		if (abs(ballSpeed.x) < playerSpeed) {
			if (ballPos.x > LT.x + width / 2) {
				ballSpeed.x = playerSpeed;
			}
			else if (ballPos.x < LT.x + width / 2) {
				ballSpeed.x = -playerSpeed;
			}
		}
		else {
			if (ballPos.x > LT.x + width / 2) {
				ballSpeed.x = abs(ballSpeed.x) * .6f * BOOST;
			}
			else if (ballPos.x < LT.x + width / 2) {
				ballSpeed.x = -abs(ballSpeed.x) * .6f * BOOST;
			}
		}
		ballSpeed.y = -abs(ballSpeed.y) * 1.1f;
	}
}

void Globular::setSpeedX(const float speedX) {
	ballSpeed.x = speedX;
}

void Globular::setSpeedY(const float speedY) {
	ballSpeed.y = speedY;
}

void Globular::setSpeed(const Vector2f &speed) {
	ballSpeed = speed;
}

void Globular::setPosition(const Vector2f &pos) {
	ball->setPosition(pos);
}

void Globular::setPosition(const float posX, const float posY) {
	ball->setPosition(posX, posY);
}

void Globular::setRadius(const float radius) {
	ball->setRadius(radius);
}

bool Globular::isBroke() const {
	return bBroke;
}

const Vector2f & Globular::getSpeed() const {
	return ballSpeed;
}

float Globular::getRadius() const {
	return ball->getRadius() + ball->getOutlineThickness();
}

const Vector2f & Globular::getPosition() const {
	return ball->getPosition();
}

Vector2f Globular::getPreviousPos() const {
	return ball->getPosition() - ballSpeed;
}

void Globular::resetBall() {
	ballSpeed.x = (prng(150) % 150 * .01f + 1.5f) * (rng() < 0 ? 1 : -1);
	ballSpeed.y = 1.5f * (rng() < 0 ? 1 : -1);
}

void Globular::Side::reset() {
	bLeft = false;
	bRight = false;
	bTop = false;
	bBottom = false;
}
