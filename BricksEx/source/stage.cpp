#include "stage.h"
#include "hud.h"
#include "particleSystem.h"
#include "event/mouse/mouseEvent.h"
#include "event/eventListener.h"
#include "event/keyboard/keyEvent.h"
#include "definition/gameState.h"
#include "definition/utility.h"
#include "manager/audioManager.h"
#include "stuff/obstacle.h"
#include "stuff/ball.h"
#include "stuff/player.h"
#include "stuff/brick.h"

Stage::Stage() :
	hud(new HUD),
	player(new Player),
	ball(new item::Ball),
	brick(new item::Brick),
	obstacle(new Obstacle),
	mouseLight(new ParticleSystem(2000)) {
	// presettle mainBall's position
	ball->followPlayer(player->getMainPlayerTopCenterPos());
	addChild({ hud, player, ball, brick, obstacle, mouseLight });
	using namespace std::placeholders;
	// addListener(std::make_shared<game::EventListener<game::KeyPressedEvent>>(std::bind(&Stage::onKeyPressed, this, _1)));
	addListener(std::make_shared<game::EventListener<game::KeyPressedEvent>>(std::bind(&Stage::onKeyPressed, this, _1)));
	addListener(std::make_shared<game::EventListener<game::KeyReleasedEvent>>(std::bind(&Stage::onKeyReleased, this, _1)));
	addListener(std::make_shared<game::EventListener<game::MouseEnteredEvent>>(std::bind(&Stage::onMouseEntered, this, _1)));
	addListener(std::make_shared<game::EventListener<game::MouseLeftEvent>>(std::bind(&Stage::onMouseLeft, this, _1)));
	addListener(std::make_shared<game::EventListener<game::MouseMovedEvent>>(std::bind(&Stage::onMouseMoved, this, _1)));
	addListener(std::make_shared<game::EventListener<game::MousePressedEvent>>(std::bind(&Stage::onMouseButtonPressed, this, _1)));
}

Stage::~Stage() {
	AudioManager::getInstance().bgmusic.stop();
	AudioManager::getInstance().sound1.stop();
}

void Stage::update(const float updateRatio) {
	mouseLight->update(updateRatio);
	if (GameState::pause) {
		return;
	}

	ball->initializeBall();
	for (size_t i = 0; i < SLICE; ++i) {
		player->update(ball->getMainBallPosition(), ball->getMainBallRadius(), updateRatio);
		if (GameState::start) {
			brick->update(*ball, updateRatio);
			obstacle->update(*ball, updateRatio);
			ball->update(player->getMainPlayerDP(), updateRatio);
		}
		else {
			ball->followPlayer(player->getMainPlayerTopCenterPos());
			obstacle->restart();
		}
	}
}

void Stage::onKeyPressed(game::KeyPressedEvent & event) {
	if (event.code == sf::Keyboard::P) {
		GameState::pause = !GameState::pause;
		GameState::lock = !GameState::lock;
	}

	if (GameState::lock) {
		return;
	}
	else {
		if (event.code == sf::Keyboard::G) {
			GameState::start = true;
		}
	}
}

void Stage::onKeyReleased(game::KeyReleasedEvent & event) {

}

void Stage::onMouseEntered(game::MouseEnteredEvent &) {
	mouseLight->startEmit();
}

void Stage::onMouseLeft(game::MouseLeftEvent &) {
	mouseLight->stopEmit();
}

void Stage::onMouseMoved(game::MouseMovedEvent & event) {
	mouseLight->setEmitPosition(sf::Vector2f(static_cast<float>(event.x), static_cast<float>(event.y)));
}

void Stage::onMouseButtonPressed(game::MousePressedEvent & event) {
	if (!GameState::lock) {
		if (event.button == sf::Mouse::Left) {
			// debugging feature
			GameState::start = true;
		}
		// debugging feature
		else if (event.button == sf::Mouse::Right) {
			GameState::start = false;
			GameState::ready = false;
		}
	}
}

