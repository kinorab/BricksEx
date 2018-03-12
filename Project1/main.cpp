#include "ellipse.h"
#include "particleSystem.h"
#include "brick.h"
#include "define.h"
#include "hud.h"
#include "obstacle.h"
#include "UIFactory.h"
#include <atomic>
#include <iostream>
#include <map>
#include <stdexcept>
#include <queue>
#include <mutex>
#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace std;
using namespace item;

static queue<Event> gameEventQueue;
static mutex gameEventQueueMutex;

void renderThread(RenderWindow *window, atomic<bool> *done) {

	window->setActive(true);
	static float blockLength = 100.f;
	static float incre1 = 3.f;

	Obstacle obstacles(2
		, { Vector2f(blockLength, blockLength * incre1), Vector2f(STAGE_WIDTH - blockLength * 2, blockLength * incre1) }
	, { Vector2f(blockLength, blockLength * incre1), Vector2f(blockLength, blockLength * incre1) });
	obstacles.setBlockColor(0, Color::Black, Color::Blue, Color::Black, Color::Black);
	obstacles.setBlockColor(1, Color::Green, Color::Black, Color::Cyan, Color::Black);
	obstacles.setBlockSpeed(0, 1.5f);
	obstacles.setBlockSpeed(1, -1.5f);

	Player player(5.5f);

	Ball ball(player);

	Brick bricks(1, 60.f, 25.f, Vector2f(0.8f, 2.f), 3.f);
	bricks.setBrickColor(Color(static_cast<Uint8>(255), static_cast<Uint8>(183), static_cast<Uint8>(197)));

	ParticleSystem mouseLight(2000);
	Vector2i localPosition;
	Mouse::setPosition(static_cast<Vector2i>(Vector2f(STAGE_WIDTH / 2, STAGE_HEIGHT / 2)), *window);
	Sound sound1;
	Music bgmusic;
	SoundBuffer buffer1;

	HUD hud;

	static float bufferBgVolume = 100.0f;
	static float bufferVolume1 = 50.0f;

	try {
		// if memory violation happen, reset the lib connector of project (-d have something bug)
		if (!buffer1.loadFromFile("s1.wav")) {
			throw runtime_error("Cannot get the sound file.");
		}
		else if (!bgmusic.openFromFile("bg.wav")) {
			throw runtime_error("Cannot get the music file.");
		}
		sound1.setBuffer(buffer1);
		sound1.setVolume(bufferVolume1);
		/*bgmusic.play();
		bgmusic.setLoop(true);*/
	}
	catch (runtime_error  &ex) {
		cout << "Runtime_error: " << ex.what() << endl;
	}

	Time elapsed = milliseconds(0);
	Clock clock;
	bool finishing = false;
	map<Keyboard::Key, bool> keyDown;

	while (!finishing) {

		Event getEvent;
		Vector2i mousePosition = Mouse::getPosition(*window);

		while (!gameEventQueue.empty()) {

			gameEventQueueMutex.lock();
			getEvent = gameEventQueue.front();
			gameEventQueue.pop();
			gameEventQueueMutex.unlock();

			if ((getEvent.type == Event::KeyPressed
				|| getEvent.type == Event::KeyReleased)
				&& keyDown.find(getEvent.key.code) == keyDown.end()
				) {
				keyDown.insert({ getEvent.key.code, false });
			}

			if (getEvent.type == Event::KeyPressed) {
				if (keyDown[getEvent.key.code])	continue;
				keyDown[getEvent.key.code] = true;
				if (GameState::lock) continue;
				if (keyDown[Keyboard::G]
					|| keyDown[Keyboard::F]
					|| keyDown[Keyboard::R]
					|| keyDown[Keyboard::D]
					|| keyDown[Keyboard::E]
					|| keyDown[Keyboard::P]) {
					GameState::lock = true;
					if (keyDown[Keyboard::G]) {
						GameState::start = true;
					}

					if (keyDown[Keyboard::P]) {
						GameState::pause = !GameState::pause;
					}
				}
			}
			else if (getEvent.type == Event::KeyReleased) {
				if (!keyDown[getEvent.key.code]) continue;
				keyDown[getEvent.key.code] = false;

				if (!keyDown[Keyboard::G]
					&& !keyDown[Keyboard::F]
					&& !keyDown[Keyboard::R]
					&& !keyDown[Keyboard::D]
					&& !keyDown[Keyboard::E]
					&& !keyDown[Keyboard::P]) {
					GameState::lock = false;
				}
			}

			if (getEvent.type == Event::MouseButtonPressed) {
				if (getEvent.mouseButton.button == Mouse::Left) {
					GameState::start = true;
				}
				// when game released, please comment this
				else if (getEvent.mouseButton.button == Mouse::Right) {
					GameState::start = false;
					GameState::ready = false;
				}
			}
			else if (getEvent.type == Event::MouseEntered) {
				GameState::light = true;
			}
			else if (getEvent.type == Event::MouseLeft) {
				GameState::light = false;
			}

			if (getEvent.type == Event::Closed) {
				bgmusic.stop();
				sound1.stop();
				finishing = true;
			}
		}

		// update fixed 5 frames
		elapsed += clock.restart();
		if (elapsed.asSeconds() > 0.05f) {
			elapsed = seconds(0.05f);
		}

		// updateSpan: milliseconds
		static constexpr float updateSpan = 13.0f;
		while (elapsed.asSeconds() * 1000.0f > updateSpan) {
			if (!GameState::pause) {
				player.playerMove();
				ball.ballEnableMove(player, sound1);
				if (GameState::start) {
					obstacles.enable(ball);
					bricks.enable(ball);
					ball.move(player);
					if (bricks.isEmpty()) {
						GameState::ready = false;
						GameState::start = false;
						GameState::reflash = true;
						cout << "Finished stage: " << stage++ << "!!!" << endl;
						bricks.reset(stage);
						bricks.setBrickColor(Color(static_cast<Uint8>(rng() % 255), static_cast<Uint8>(rng() % 255), static_cast<Uint8>(rng() % 255)));
					}
				}
				else {
					ball.followPlayer(player);
					if (!GameState::ready) {
						obstacles.reset();
						GameState::ready = true;
					}
				}
			}
			mouseLight.setEmitter(window->mapPixelToCoords(mousePosition));
			mouseLight.update(updateSpan);
			elapsed -= seconds(updateSpan / 1000.0f);
		}

		// render
		window->clear(Color::White);
		window->draw(obstacles);
		window->draw(player);
		window->draw(ball);
		window->draw(bricks);
		window->draw(hud);
		window->draw(mouseLight);
		window->display();
	}
	// finalize...
	*done = true;
}

int main() {
	ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 6;
	settings.majorVersion = 4;
	settings.minorVersion = 1;

	RenderWindow window(VideoMode(static_cast<int>(GAME_WIDTH), static_cast<int>(GAME_HEIGHT)), "Pigject", Style::Titlebar | Style::Close, settings);
	window.setMouseCursorVisible(false);
	window.setPosition(Vector2i(window.getPosition().x, 20));
	window.setVerticalSyncEnabled(true);
	window.setActive(false);
	atomic <bool> done(false);
	thread subthread(renderThread, &window, &done);
	// main thread wait for event and push to queue
	Event event;
	while (!done && window.waitEvent(event)) {

		gameEventQueueMutex.lock();
		gameEventQueue.push(event);
		gameEventQueueMutex.unlock();
	}

	// finalize...
	window.close();
	subthread.join();
	system("pause");
	return 0;
}
