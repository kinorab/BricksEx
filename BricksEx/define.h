#pragma once

#include <SFML/Graphics/Rect.hpp>

extern unsigned int level;

constexpr float PI = 3.141592654f;
constexpr float LIFETIME = 1500.f;
constexpr float RESETTIME = 25.f;
constexpr float BOOST = 2.5f;
constexpr float MAXSPEED = 20.f;
constexpr float GAME_WIDTH = 1500.f;
constexpr float GAME_HEIGHT = 900.f;
constexpr float LEVEL_WIDTH = 1200.f;
constexpr float LEVEL_HEIGHT = 900.f;
constexpr size_t SLICE = 10;
extern float MAINPLAYERSPEED;

namespace GameState {
	extern bool start;
	extern bool ready;
	extern bool light;
	extern bool finishLevel;
	extern bool pause;
	extern bool lock;
};

extern const int rng();
extern const size_t Prng();

namespace game {

	template <typename T>
	extern const bool intersects(const sf::Rect<T> &a, const sf::Rect<T> &b);
	#include "rect.inl"
}
