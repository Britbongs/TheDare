#include "Spawner.h"

Spawner::Spawner(int enemyCount, sf::Vector2i position, vector<Enemy>* enemies, const TiledMap* const map)
	: enemyCount_(enemyCount), enemies_(enemies), spawnerPosition_(position), map_(map)
{

}

void Spawner::spawnEnemies()
{
	bool found(false);
	int count(0);

	while (!found && count < enemies_->size())
	{//find the first enemy who isn't alive
		if ((*enemies_)[count].getAlive() == false)
			found = true;
		++count;
	}
	generateFreeLocations();
	if (found)
	{
		--count;

		for (int i(count); i < (count + enemyCount_) && i < enemies_->size(); ++i)
		{
			placeEnemy((*enemies_)[i]);
			(*enemies_)[i].setAlive(true);
		}
	}

}


void Spawner::placeEnemy(Enemy& e)
{//Function to place an enemy in a grid location around a spawner
	if (locations_.size() > 0)
	{
		sf::Vector2i gridPos(0, 0);
		sf::Vector2f worldPos(0.f, 0.f);

		switch (locations_[locations_.size() - 1])
		{
		case UP_LEFT: gridPos = addVector(spawnerPosition_, sf::Vector2i(-1, -1)); break;

		case UP: gridPos = addVector(spawnerPosition_, sf::Vector2i(0, -1)); break;

		case UP_RIGHT: gridPos = addVector(spawnerPosition_, sf::Vector2i(1, -1)); break;

		case RIGHT: gridPos = addVector(spawnerPosition_, sf::Vector2i(1, 0)); break;

		case DOWN_RIGHT: gridPos = addVector(spawnerPosition_, sf::Vector2i(1, 1)); break;

		case DOWN: gridPos = addVector(spawnerPosition_, sf::Vector2i(0, 1)); break;

		case DOWN_LEFT: gridPos = addVector(spawnerPosition_, sf::Vector2i(-1, 1)); break;

		case LEFT: gridPos = addVector(spawnerPosition_, sf::Vector2i(-1, 0)); break;
		
		default: gridPos = spawnerPosition_;
		}

		worldPos.x = static_cast<float> (gridPos.x * gconsts::Gameplay::TILESIZE);
		worldPos.y = static_cast<float> (gridPos.y * gconsts::Gameplay::TILESIZE);
		worldPos.x += gconsts::Gameplay::TILESIZE / 2.f;
		worldPos.y += gconsts::Gameplay::TILESIZE / 2.f;
		e.setPosition(worldPos);
		locations_.pop_back();
	}
}

void Spawner::generateFreeLocations()
{//Function to find all free locations around a tile

	//Check up left
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(-1, -1))))
		locations_.push_back(UP_LEFT);

	//Check up 
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(0, -1))))
		locations_.push_back(UP);

	//Check up right
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(1, -1))))
		locations_.push_back(UP_RIGHT);

	//Check right
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(1, 0))))
		locations_.push_back(RIGHT);

	//Check down right
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(1, 1))))
		locations_.push_back(DOWN_RIGHT);

	//Check down
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(0, 1))))
		locations_.push_back(DOWN);

	//Check down left
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(-1, 1))))
		locations_.push_back(DOWN_LEFT);

	//Check left
	if (!map_->isTileBlocked(addVector(spawnerPosition_, sf::Vector2i(-1, 0))))
		locations_.push_back(LEFT);

	if (!map_->isTileBlocked(spawnerPosition_))
		locations_.push_back(CENTRE);
}