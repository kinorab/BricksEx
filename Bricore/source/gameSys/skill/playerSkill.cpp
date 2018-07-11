#include "playerSkill.h"
#include "skillHandler.h"
#include "../effect/entireEffect.h"
#include "../bar/energyBar.h"
#include "../../manager/textureManager.h"
#include "../../definition/gameState.h"
#include <SFML/Graphics.hpp>

using namespace game;

size_t PlayerSkill::uMaxCarry(3);
size_t PlayerSkill::uCurrentCarry(0);
size_t PlayerSkill::uMaxOnField(2);
size_t PlayerSkill::uCurrentOnField(0);
SkillHandler<PlayerSkill> PlayerSkill::handler;
PlayerSkill::SkillKey PlayerSkill::key;

PlayerSkill::PlayerSkill(const Kind skillName, const sf::Time duration
	, std::vector<Effect::Kind> && effects, std::vector<Attribute::Kind> && attributes
	, const bool autoUse, const bool exist, const std::shared_ptr<EnergyBar> energyBar)
	: skill(SkillContent{ skillName, State::None, nullptr })
	, SkillSystem(duration, autoUse, exist)
	, m_energyBar(std::move(energyBar))
	, bInitialize(false) {
	// initialize effects
	std::for_each(effects.begin(), effects.end(), [&](const Effect::Kind effect) {
		skillEffects.push_back(std::make_shared<EntireEffect>(effect, this));
	});
	// initialize attributes
	std::for_each(attributes.begin(), attributes.end(), [&](const Attribute::Kind element) {
		skillAttributes.push_back(std::make_shared<Attribute>(element));
	});
}

void PlayerSkill::initialize() {
	if (bInitialize) return;
	handler.insert(shared_from_this());
	bInitialize = true;
}

void PlayerSkill::handleSkill(const sf::Event * const event) {
	State currentState = skill.currentState;
	if (status != Status::Selected || currentGameState == GameState::LEVEL_FINISHED) return;
	using namespace std::placeholders;
	switch (currentState) {
	case State::OnCharging:
		if (!m_energyBar->isFull() || bLocked) break;
		if (handler.tryEnterField(std::bind(&PlayerSkill::setState, this, _1))) {
			m_energyBar->clear();
		}
		break;
	case State::OnFirstField:
		if (bSilenced) break;
		if (event->key.code == key.playerSkill || bAutoUse) setState(State::Using);
		break;
	case State::OnSecondField:
		if (bLocked) break;
		if (handler.tryForward(currentState, std::bind(&PlayerSkill::setState, this, _1))) {}
		break;
	case State::OnThirdField:
		if (bLocked) break;
		if (handler.tryForward(currentState, std::bind(&PlayerSkill::setState, this, _1))) {}
		break;
	case State::OnFourthField:
		if (bLocked) break;
		if (handler.tryForward(currentState, std::bind(&PlayerSkill::setState, this, _1))) {}
		break;
	case State::None:
		if (handler.tryAppear(shared_from_this())) {}
		break;
	case State::Using:
		useSkill();
		if (elapsed()) {
			exhausted();
			setState(State::None);
		}
		break;
	default:
		throw std::exception("Some bug in handleSkill.");
		break;
	}
	if (event->type != sf::Event::KeyPressed) return;
	if (event->key.code == key.playerSkillSwap) {
		if (handler.trySwap()) {
		}
		else {
		}
	}
}

void PlayerSkill::handleSelect(const sf::Event * const event) {
	if (status == Status::None || event->type != sf::Event::MouseButtonPressed 
		|| currentGameState != GameState::LEVEL_FINISHED) return;
	if (skill.context->getGlobalBounds().contains(static_cast<float>(event->mouseButton.x)
		, static_cast<float>(event->mouseButton.y))) {
		if (uCurrentCarry < uMaxCarry && SkillSystem::selectOn()) {
			++uCurrentCarry;
			return;
		}
		if (SkillSystem::selectOff()) {
			--uCurrentCarry;
			return;
		}
	}
}

bool PlayerSkill::containsPoint(const sf::Vector2f & point) const {
	if (skill.currentState == State::None) return false;
	return skill.context->getGlobalBounds().contains(point);
}

std::shared_ptr<sf::Drawable> PlayerSkill::getDrawable() const {
	return std::const_pointer_cast<sf::Drawable>(std::static_pointer_cast<const sf::Drawable>(shared_from_this()));
}

void PlayerSkill::loadStatePreview(const std::map<State, std::string> &fileName, const bool isSmooth) {
	std::for_each(fileName.begin(), fileName.end(), [&](const std::pair<State, std::string> & file) {
		statePreviews.emplace(file.first, TextureManager::getInstance().get(file.second));
		statePreviews.at(file.first)->setSmooth(isSmooth);
	});
}

void PlayerSkill::extendCarry(const size_t number) {
	uMaxCarry += number;
}

void PlayerSkill::extendField(const size_t number) {
	if (number + uMaxOnField > static_cast<size_t> (maxField)) {
		throw std::out_of_range("Excess max field number");
	}
	uMaxOnField += number;
}

void PlayerSkill::resetKey(const sf::Keyboard::Key playerSkill, const sf::Keyboard::Key playerSkillSwap
	, const sf::Keyboard::Key switchToPrevChargingSkill, const sf::Keyboard::Key switchToNextChargingSkill) {
	key = { playerSkill, playerSkillSwap, switchToPrevChargingSkill, switchToNextChargingSkill };
}

void PlayerSkill::setState(const State nextState) {
	// set field number
	if (skill.currentState == State::OnCharging) {
		if (uCurrentOnField > uMaxOnField) throw std::invalid_argument("Too many skills on field.");
		++uCurrentOnField;
	}
	else if (nextState == State::Using) {
		if (uCurrentOnField == 0) throw std::invalid_argument("Field no have any skills.");
		--uCurrentOnField;
	}
	// set skill context
	if (nextState == State::None) {
		skill.context = nullptr;
	}
	else {
		skill.context.reset(new sf::Sprite(*statePreviews.at(nextState)));
	}
	// set skill state
	skill.currentState = nextState;
}

void PlayerSkill::swapSkill(const std::shared_ptr<PlayerSkill> & other) {
	State state = skill.currentState;
	State inputState = other->skill.currentState;
	statePreviews.swap(other->statePreviews);
	std::swap(skill, other->skill);
	setState(inputState);
	other->setState(state);
}

size_t PlayerSkill::getMaxCarry() {
	return uMaxCarry;
}

size_t PlayerSkill::getCurrentCarry() {
	return uCurrentCarry;
}

size_t PlayerSkill::getMaxOnField() {
	return uMaxOnField;
}

size_t PlayerSkill::getCurrentOnField() {
	return uCurrentOnField;
}

bool PlayerSkill::isInitialize() const {
	return bInitialize;
}

PlayerSkill::State PlayerSkill::getState() const {
	return skill.currentState;
}

SkillKind<PlayerSkill>::Kind PlayerSkill::getSkillName() const {
	return skill.name;
}

PlayerSkill::~PlayerSkill() {
}

void PlayerSkill::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	if (skill.currentState == State::None) return;
	target.draw(*skill.context, states);
}
