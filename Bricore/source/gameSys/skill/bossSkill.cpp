#include "bossSkill.h"
#include "skillHandler.h"
#include "../effect/entireEffect.h"
#include "../bar/rageBar.h"
#include "../../definition/gameState.h"
#include <SFML/Graphics.hpp>

using namespace game;

SkillHandler<BossSkill> BossSkill::handler;

BossSkill::BossSkill(const Kind skillName, const sf::Time &duration
	, const std::vector<Effect::Kind> &effects
	, const std::vector<Attribute::Kind> &attributes, const std::shared_ptr<RageBar> rageBar
	, const bool exist)
	: SkillSystem(duration, true, exist)
	, skill(skillName, SkillContent{ State::None, nullptr })
	, cache(nullptr)
	, rageBar(std::move(rageBar)){
	std::for_each(effects.begin(), effects.end(), [&](const Effect::Kind effect) {
		skillEffects.push_back(std::make_shared<EntireEffect>(effect, this));
	});
	std::for_each(attributes.begin(), attributes.end(), [&](const Attribute::Kind element) {
		skillAttributes.push_back(std::make_shared<Attribute>(element));
	});
	handler.insert(shared_from_this());
}

void BossSkill::handleSkill(const sf::Event * const event) {
}

void BossSkill::handleSelect(const sf::Event * const event) {
}

void BossSkill::loadPreview(const std::string & fileName) {
	cache.reset(new sf::Texture);
	cache->loadFromFile(fileName);
	skill.second.preview.reset(new sf::Sprite(*cache));
}

SkillState<BossSkill>::State BossSkill::getState() const {
	return skill.second.currentState;
}

SkillKind<BossSkill>::Kind BossSkill::getSkillName() const {
	return skill.first;
}

BossSkill::~BossSkill() {
}

void BossSkill::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= getTransform();
	if (skill.second.preview) {
		target.draw(*skill.second.preview, states);
	}
}

void BossSkill::setState(const State state) {
	skill.second.currentState = state;
}
