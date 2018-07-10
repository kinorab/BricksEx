#include "game.h"
#include "stage.h"
#include "root.h"
#include "UI/graphics.h"
#include "manager/audioManager.h"
#include "definition/gameState.h"
#include "definition/utility.h"
#include "gameSys/level/level.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <future>

using namespace game;

Game::Game()
	: graph(new Graphics)
	, root(new Root) {
	//if (saveData.exist() && root.chooseSave(saveData)) {
	//	level.reset(saveData);
	//}
	//else { 
	level.reset(new Level(Mode::NoChoose, Diffculty::NoChoose));
	//}
	stage.reset(new Stage(level, root));
	window.reset(new sf::RenderWindow(sf::VideoMode(static_cast<size_t>(GAME_WIDTH), static_cast<size_t>(GAME_HEIGHT)),
		"Bricore", sf::Style::Close, graph->getSettings()));
	level->bDefaultControlKeySettled = true;
}

Game::~Game() = default;

void Game::run() {
	settleWindow();
	AudioManager::getInstance().initialize();
	std::future<void> renderThread = std::async(std::launch::async, &Game::renderFunc, this);
	sf::Event nextEvent;
	while (renderThread.wait_for(std::chrono::seconds(0)) != std::future_status::ready && window->waitEvent(nextEvent)) {
		eventQueue.push(nextEvent);
	}
	window->close();
}

void Game::settleWindow() {
	window->setMouseCursorVisible(false);
	window->setVerticalSyncEnabled(true);
	window->setPosition(sf::Vector2i(window->getPosition().x, 20));
	ImmAssociateContext(window->getSystemHandle(), 0);
	//window.setIcon(graph->getIconSize().x, graph->getIconSize().y, graph->getIcon());
	window->setActive(false);
}

void Game::renderFunc() {
	float elapsed = 0;
	float renderElapsed = 0;
	sf::Clock clock;
	for (bool finishing = false; !finishing;) {
		// display in milliseconds
		constexpr float updateSpan = 10.f;
		const float distribute = clock.restart().asSeconds() * 1000.f;
		handleEvents(finishing);
		// maximum elapsed cap
		elapsed = std::min<float>(elapsed + distribute, updateSpan * 1.5f);
		while (elapsed > 0.0f) {
			float updateRatio = std::min<float>(elapsed, updateSpan) / updateSpan;
			auto rootFunc = std::async(std::launch::async, &Root::update, root, updateRatio);
			auto stageFunc = std::async(std::launch::async, &Stage::update, stage, updateRatio);
			elapsed -= updateSpan * updateRatio;
		}
		// maximum render elapsed cap
		renderElapsed = std::min<float>(renderElapsed + distribute, graph->getFrameSpan() * 1.5f);
		if (renderElapsed >= graph->getFrameSpan()) {
			window->draw(*stage);
			window->display();
			renderElapsed -= graph->getFrameSpan();
		}
	}
	// finalize...
	window->setActive(false);
}

void Game::handleEvents(bool & finishing) {
	while (!eventQueue.empty()) {
		sf::Event currentEvent = eventQueue.pop();
		root->handle(currentEvent);
		stage->handle(currentEvent);
		if (currentEvent.type == sf::Event::Closed) {
			finishing = true;
		}
	}
}