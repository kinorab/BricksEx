#include "SFMLMouseHandler.h"
#include "definition/utility.h"
#include "event/mouse/mouseButtonEvent.h"
#include "event/mouse/mouseMovedEvent.h"
#include "event/mouse/mouseEnteredEvent.h"
#include "event/mouse/mouseLeftEvent.h"

namespace game {
	SFMLMouseHandler::SFMLMouseHandler(const sf::Vector2i clientSize) :
		clientSize(clientSize) {
	}

	void SFMLMouseHandler::handle(const sf::Event & event, Container & root) {
		if (event.type == sf::Event::MouseMoved) {
			handleMouseMove(event, root);
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			handleMouseButtonPressed(event, root);
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			handleMouseButtonReleased(event, root);
		}
		else if (event.type == sf::Event::MouseLeft) {
			handleMouseLeft(event, root);
		}
	}

	void SFMLMouseHandler::handleMouseButtonPressed(const sf::Event & event, Container & root) {
		if (!previousContactNode) {
			return;
		}

		MouseButtonEvent gameEvent(typeid(MouseButtonEvent::Pressed), event.mouseButton);
		previousContactNode->dispatchEvent(gameEvent);
	}

	void SFMLMouseHandler::handleMouseButtonReleased(const sf::Event & event, Container & root) {
		if (!previousContactNode) {
			return;
		}

		MouseButtonEvent gameEvent(typeid(MouseButtonEvent::Released), event.mouseButton);
		previousContactNode->dispatchEvent(gameEvent);
	}

	void SFMLMouseHandler::handleMouseLeft(const sf::Event & event, Container & root) {
		sf::Event newEvent;
		newEvent.type = sf::Event::MouseMoved;
		newEvent.mouseMove = { -1, -1 };
		handleMouseMove(newEvent, root);
	}

	void SFMLMouseHandler::handleMouseMove(const sf::Event & event, Container & root) {
		sf::Vector2i mousePosition = { event.mouseMove.x, event.mouseMove.y };
		std::shared_ptr<InteractiveObject> contactNode;
		if (mousePosition.x < 0 || mousePosition.x > clientSize.x
			|| mousePosition.y < 0 || mousePosition.y > clientSize.y) {
			contactNode = nullptr;
		}
		else {
			contactNode = root.getObjectUnderPoint(sf::Vector2f(mousePosition));
			if (contactNode) {
				MouseMovedEvent event(event.mouseMove);
				contactNode->dispatchEvent(event);
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
				MouseLeftEvent gameEvent;
				node->dispatchEvent(gameEvent);
			});

			std::for_each(currentNodes.begin(), currentNodes.end() - sameNodeCount,
				[&](std::shared_ptr<InteractiveObject> & node) {
				MouseEnteredEvent gameEvent;
				node->dispatchEvent(gameEvent);
			});

			previousContactNode = contactNode;
		}
	}
}