#include "mouseHandler.h"
#include "../definition/utility.h"
#include "../definition/gameState.h"
#include "../interact/container.h"

namespace game {

	MouseHandler::MouseHandler() {
	}

	void MouseHandler::handle(const sf::Event & event, Container & target) {
		if (event.type == sf::Event::MouseMoved) {
			handleMouseMove(event, target);
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			handleMouseButtonPressed(event, target);
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			handleMouseButtonReleased(event, target);
		}
		else if (event.type == sf::Event::MouseLeft) {
			handleMouseLeft(event, target);
		}
	}

	void MouseHandler::handleMouseButtonPressed(const sf::Event & event, Container & target) {
		if (!previousContactNode) {
			return;
		}

		previousContactNode->dispatchEvent(MousePressedEvent(event.mouseButton));
	}

	void MouseHandler::handleMouseButtonReleased(const sf::Event & event, Container & target) {
		if (!previousContactNode) {
			return;
		}

		previousContactNode->dispatchEvent(MouseReleasedEvent(event.mouseButton));
	}

	void MouseHandler::handleMouseLeft(const sf::Event & event, Container & target) {
		sf::Event newEvent;
		newEvent.type = sf::Event::MouseMoved;
		newEvent.mouseMove = { -1, -1 };
		handleMouseMove(newEvent, target);
	}

	void MouseHandler::handleMouseMove(const sf::Event & event, Container & target) {
		const sf::Vector2i & mousePosition{ event.mouseMove.x, event.mouseMove.y };
		std::shared_ptr<InteractiveObject> contactNode;
		if (mousePosition.x < 0 || mousePosition.x > GAME_WIDTH
			|| mousePosition.y < 0 || mousePosition.y > GAME_HEIGHT) {
			contactNode = nullptr;
		}
		else {
			contactNode = target.getObjectUnderPoint(sf::Vector2f(mousePosition));
			if (contactNode) {
				contactNode->dispatchEvent(MouseMovedEvent(event.mouseMove));
			}
		}

		if (contactNode != previousContactNode) {
			std::vector<std::shared_ptr<InteractiveObject>> previousNodes;
			for (std::shared_ptr<InteractiveObject> node = previousContactNode; node;) {
				previousNodes.push_back(node);
				Container * parent = node->getParent();
				if (parent) {
					node = parent->shared_from_this();
				}
				else {
					node = nullptr;
				}
			}

			std::vector<std::shared_ptr<InteractiveObject>> currentNodes;
			for (std::shared_ptr<InteractiveObject> node = contactNode; node;) {
				currentNodes.push_back(node);
				Container * parent = node->getParent();
				if (parent) {
					node = parent->shared_from_this();
				}
				else {
					node = nullptr;
				}
			}

			int sameNodeCount = 0;
			if (previousContactNode && contactNode) {
				while (*(previousNodes.rbegin() + sameNodeCount) == *(currentNodes.rbegin() + sameNodeCount)) {
					sameNodeCount += 1;
				}
			}

			std::for_each(previousNodes.begin(), previousNodes.end() - sameNodeCount,
				[&](std::shared_ptr<InteractiveObject> & node) {
				node->dispatchEvent(MouseLeftEvent());
			});

			std::for_each(currentNodes.begin(), currentNodes.end() - sameNodeCount,
				[&](std::shared_ptr<InteractiveObject> & node) {
				node->dispatchEvent(MouseEnteredEvent());
			});

			previousContactNode = contactNode;
		}
	}
}