#include "define.h"
#include "block.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace sf;
using namespace item;

Block::Block(const Vector2f & position, const float width, const float height)
	: VertexArray(Quads, 4), position(position), oriPos(position), width(width), height(height) {
	setBlockVertice();
}

Block::Block() {}

void Block::setVerticeColor(const Color & color) {

	for (size_t i = 0; i < getVertexCount(); ++i) {

		(*this)[i].color = color;
	}
}

void Block::setVerticeColor(const Color & c1, const Color & c2, const Color & c3, const Color & c4) {

	(*this)[0].color = c1;
	(*this)[1].color = c2;
	(*this)[2].color = c3;
	(*this)[3].color = c4;
}

void Block::setWidth(const float width) {
	if (width > 0) {
		this->width = width;
		setBlockVertice();
	}
	else {
		cout << "Invalid width setting." << endl;
	}
}

void Block::setHeight(const float height) {
	if (height > 0) {
		this->height = height;
		setBlockVertice();
	}
	else {
		cout << "Invalid height setting." << endl;
	}
}

void Block::resetPosition() {

	position = oriPos;
	setBlockVertice();
}

void Block::setSpeed(const float speedX, const float speedY) {
	speed.x = speedX;
	speed.y = speedY;
}

void Block::setSpeed(const Vector2f & speed) {
	this->speed = speed;
}

// all change direct by using abs() to prevent stuck inside the block
void Block::enable(CircleShape & ball, float &ballSpeedX, float &ballSpeedY) {

	FloatRect blockBounds = getBounds();
	FloatRect ballBounds = ball.getGlobalBounds();
	FloatRect leftBlock(Vector2f(blockBounds.left, blockBounds.top + ball.getRadius())
		, Vector2f(ballBounds.width, blockBounds.height - ball.getRadius() * 2));
	FloatRect rightBlock(Vector2f(blockBounds.left + blockBounds.width - ballBounds.width, blockBounds.top + ball.getRadius())
		, Vector2f(ballBounds.width, blockBounds.height - ball.getRadius() * 2));
	FloatRect topBlock(Vector2f(blockBounds.left + ball.getRadius(), blockBounds.top)
		, Vector2f(blockBounds.width - ball.getRadius() * 2, ballBounds.height));
	FloatRect bottomBlock(Vector2f(blockBounds.left + ball.getRadius(), blockBounds.top + blockBounds.height - ballBounds.height)
		, Vector2f(blockBounds.width - ball.getRadius() * 2, ballBounds.height));

	if (ballBounds.intersects(leftBlock)) {
		ballSpeedX = -abs(ballSpeedX);
	}
	else if (ballBounds.intersects(rightBlock)) {
		ballSpeedX = abs(ballSpeedX);
	}

	if (ballBounds.intersects(topBlock)) {
		ballSpeedY = -abs(ballSpeedY);
	}
	else if (ballBounds.intersects(bottomBlock)) {
		ballSpeedY = abs(ballSpeedY);
	}
	move();
}

void Block::move() {

	FloatRect blockBounds = getBounds();
	static FloatRect leftBounds(0.0f, 0.0f, -1.0f, STAGE_HEIGHT);
	static FloatRect rightBounds(STAGE_WIDTH, 0.0f, 1.0f, STAGE_HEIGHT);
	static FloatRect topBounds(0.0f, 0.0f, STAGE_WIDTH, -1.0f);
	static FloatRect bottomBounds(0.0f, STAGE_HEIGHT, STAGE_WIDTH, 1.0f);

	if (blockBounds.intersects(leftBounds)) {
		speed.x = abs(speed.x);
	}
	else if (blockBounds.intersects(rightBounds)) {
		speed.x = -abs(speed.x);
	}

	if (blockBounds.intersects(topBounds)) {
		speed.y = abs(speed.y);
	}
	else if (blockBounds.intersects(bottomBounds)) {
		speed.y = -abs(speed.y);
	}

	moveEntity();
}

const Vector2f & Block::getCurrentPosition() const {
	return position;
}

const Vector2f & Block::getOriginPosition() const {
	return oriPos;
}

const Vector2f & Block::getSpeed() const {
	return speed;
}

const float Block::getWidth() const {
	return width;
}

const float Block::getHeight() const {
	return height;
}

void Block::setBlockVertice() {

	try {
		if (width > 0 && height > 0) {

			for (size_t i = 0; i < getVertexCount(); ++i) {
				(*this)[i].position = position;
			}

			(*this)[1].position += Vector2f(width, 0.0f);
			(*this)[2].position += Vector2f(width, height);
			(*this)[3].position += Vector2f(0.0f, height);
		}
		else {
			throw domain_error("Invalid side-length for block.");
		}
	}
	catch (domain_error & ex) {
		cout << "Domain_error: " << ex.what() << endl;
	}
}

void Block::moveEntity() {

	try {
		for (size_t i = 0; i < getVertexCount(); ++i) {
			(*this)[i].position += speed;
		}
		position = (*this)[0].position;// mark new position in [0]
	}
	catch (out_of_range &ex) {
		cout << "Exception: " << ex.what() << endl;
	}
}