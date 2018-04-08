#pragma once

#include <SFML/Graphics.hpp>
#include "define.h"
#include <vector>
#include <memory>

namespace item {

	class Ball : public sf::Drawable {

	public:
		Ball();
		static void update(const sf::FloatRect &playerBounds);
		static void initializeBall();
		static void followPlayer(const sf::Vector2f &playerPos);
		static void ballCollided(const sf::FloatRect &bounds, const sf::Vector2f &speed);
		static void ballCollided(const sys::DPointf &bounds, const sf::Vector2f &speed);
		static bool isBallCollided(const sf::FloatRect &bounds);
		static bool isBallCollided(const sys::DPointf &bounds);
		static void ballDivided(const size_t numbers);
		static const float getMainBallRadius();
		static const sf::Vector2f & getMainBallPosition();

	private:
		virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
		static void collision();

		class BallContainer : public sf::Drawable {
		public:
			BallContainer();
			void update();
			void ballMove(const sf::FloatRect &);
			void setSpeedX(const float);
			void setSpeedY(const float);
			void setPosition(const sf::Vector2f &);
			void setPosition(const float, const float);
			void setRadius(const float);
			virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;

			const bool isMain() const;
			const float getSpeedX() const;
			const float getSpeedY() const;
			const float getRadius() const;
			const sf::Vector2f & getPosition() const;

			bool left = false;
			bool right = false;
			bool bottom = false;
			bool top = false;
			bool broke = false;
			bool CD = false;

		private:
			void setColor(const sf::Color &);
			void resetBall();
			sf::Vector2f ballSpeed;
			sf::Vector2f oriSpeed;
			sf::Clock countTime;
			sf::Clock CDTime;
			sf::CircleShape ball;
			bool main = false;
			bool active = false;
		};

		static bool multiple;
		static bool ballStartC;
		static bool mainSettled;
		static bool initialize;
		static std::vector<std::unique_ptr<BallContainer>> balls;
	};
}
