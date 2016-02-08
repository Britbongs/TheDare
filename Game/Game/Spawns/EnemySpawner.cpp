#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(int enemyCount, sf::Vector2i position, const TiledMap * const map)
	:enemyCount_(enemyCount), position_(position), map_(map), isTriggeredSpawn_(false), trigID_(-1), triggered_(false)
{
}

EnemySpawner::EnemySpawner(int enemyCount, sf::Vector2i position, const TiledMap * const map, bool isTriggered, int id)
	: enemyCount_(enemyCount), position_(position), map_(map), isTriggeredSpawn_(isTriggered), trigID_(id), triggered_(false)
{
}

void EnemySpawner::spawnEnemies()
{
	if (!triggered_)
	{
		bool found(false);
		int count(0);

		while (!found && count < EnemyManager::Get()->getEnemyCount())
		{
			if (EnemyManager::Get()->getEnemy(count)->getAlive() == false)
			{
				found = true;
			}
			++count;
		}

		generateFreeLocations();
		if (found)
		{
			--count;

			for (int i(count); i < (count + enemyCount_) && i < EnemyManager::Get()->getEnemyCount(); ++i)
			{
				Enemy* e(EnemyManager::Get()->getEnemy(i));
				placeEnemy(e);
				e->setAlive(true);
				if (isTriggeredSpawn_)
				{
					e->setChasing(true);
				} 

			}

		}
		triggered_ = true;
	}
}

bool EnemySpawner::isTriggeredSpawner() const
{
	return (isTriggeredSpawn_);
}

bool EnemySpawner::hasBeenTriggered() const
{
	return(triggered_);
}

void EnemySpawner::reset()
{
	triggered_ = false;
}

int EnemySpawner::getSpawnerID() const
{
	return(trigID_);
}

void EnemySpawner::placeEnemy(Enemy* e)
{
	if (locations_.size() > 0)
	{
		sf::Vector2i gridPos(0, 0);
		sf::Vector2f worldPos(0.f, 0.f);

		switch (locations_[locations_.size() - 1])
		{
		case UP_LEFT: gridPos = addVector(position_, sf::Vector2i(-1, -1)); break;

		case UP: gridPos = addVector(position_, sf::Vector2i(0, -1)); break;

		case UP_RIGHT: gridPos = addVector(position_, sf::Vector2i(1, -1)); break;

		case RIGHT: gridPos = addVector(position_, sf::Vector2i(1, 0)); break;

		case DOWN_RIGHT: gridPos = addVector(position_, sf::Vector2i(1, 1)); break;

		case DOWN: gridPos = addVector(position_, sf::Vector2i(0, 1)); break;

		case DOWN_LEFT: gridPos = addVector(position_, sf::Vector2i(-1, 1)); break;

		case LEFT: gridPos = addVector(position_, sf::Vector2i(-1, 0)); break;

		default: gridPos = position_;
		}

		worldPos.x = static_cast<float> (gridPos.x * gconsts::Gameplay::TILESIZE);
		worldPos.y = static_cast<float> (gridPos.y * gconsts::Gameplay::TILESIZE);
		worldPos.x += gconsts::Gameplay::TILESIZE / 2.f;
		worldPos.y += gconsts::Gameplay::TILESIZE / 2.f;
		e->setPosition(worldPos);
		locations_.pop_back();
	}
}

void EnemySpawner::generateFreeLocations()
{
	//Check up left
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(-1, -1))))
		locations_.push_back(UP_LEFT);

	//Check up 
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(0, -1))))
		locations_.push_back(UP);

	//Check up right
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(1, -1))))
		locations_.push_back(UP_RIGHT);

	//Check right
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(1, 0))))
		locations_.push_back(RIGHT);

	//Check down right
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(1, 1))))
		locations_.push_back(DOWN_RIGHT);

	//Check down
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(0, 1))))
		locations_.push_back(DOWN);

	//Check down left
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(-1, 1))))
		locations_.push_back(DOWN_LEFT);

	//Check left
	if (!map_->isTileBlocked(addVector(position_, sf::Vector2i(-1, 0))))
		locations_.push_back(LEFT);

	//Check centre
	if (!map_->isTileBlocked(position_))
		locations_.push_back(CENTRE);
}