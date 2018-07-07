#pragma once
#include "interact/container.h"
#include "event/game/pausedEvent.h"
#include "event/game/unpausedEvent.h"

class Ball;
class Wall;
class Obstacle;
class Player;
class ParticleSystem;
class HUD;

namespace game {
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseMovedEvent;
	class MousePressedEvent;
	class Root :
		public Container {
	public:
		Root();
		virtual ~Root();
	protected:
		virtual void update(const float updateRatio) override;
	private:
		void onKeyPressed(KeyPressedEvent & event);
		void onKeyReleased(KeyReleasedEvent & event);
		void onMouseEntered();
		void onMouseLeft();
		void onMouseMoved(MouseMovedEvent & event);
		void onMousePressed(MousePressedEvent & event);

		std::shared_ptr<HUD> hud;
		std::shared_ptr<ParticleSystem> mouseLight;
		std::shared_ptr<Player> player;
		std::shared_ptr<Ball> ball;
		std::shared_ptr<Wall> wall;
		std::shared_ptr<Obstacle> obstacle;
		bool bPaused = false;
		bool bLocked = false;
	};
}
