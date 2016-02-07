#include "Trigger.h"

//Trigger::Trigger(Spawner* spawner, sf::Vector2f position, sf::Vector2f size)
//	:spawner_(spawner), rect_(position, size),triggered_(false)
//{
//}
Trigger::Trigger(EnemySpawner* spawner, sf::Vector2f pos, sf::Vector2f size)
	:spawner_(spawner), rect_(pos,size), triggered_(false)
{

}

bool Trigger::isInTrigger(sf::Vector2f place) const
{
	return (rect_.contains(place));
}

bool Trigger::hasBeenTriggered() const
{
	return (triggered_);
}

void Trigger::activate()
{
	assert(spawner_ != nullptr);
	if (!triggered_ && !spawner_->hasBeenTriggered())
	{
		spawner_->spawnEnemies();
		triggered_ = true;
	}
}

void Trigger::decactivate()
{
	triggered_ = true;
}
