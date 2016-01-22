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

	if (found)
	{
		--count;

		for (int i(count); i < (count + enemyCount_) && i < enemies_->size(); ++i)
		{
			placeEnemy((*enemies_)[i]);
		}
	}

}


void Spawner::placeEnemy(Enemy& e)
{//Function to place an enemy in a grid location around a spawner
	if (locations_.size() > 0)
	{
		sf::Vector2i gridPos(0, 0);
		switch (locations_[locations_.size() - 1])
		{
			
		}
	}
}

void Spawner::generateFreeLocations()
{//Function to find all free locations around a tile

	//Check up left
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(-1, -1))))
		locations_.push_back(UP_LEFT);
	
	//Check up 
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(0, -1))))
		locations_.push_back(UP);

	//Check up right
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(1, -1))))
		locations_.push_back(UP_RIGHT);

	//Check right
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(1, 0))))
		locations_.push_back(RIGHT);

	//Check down right
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(1, 1))))
		locations_.push_back(DOWN_RIGHT);

	//Check down
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(0, 1))))
		locations_.push_back(DOWN);

	//Check down left
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(-1, 1))))
		locations_.push_back(DOWN_LEFT);
	
	//Check left
	if (!map_->isTileBlocked(subtractVector(spawnerPosition_, sf::Vector2i(-1, 0))))
		locations_.push_back(LEFT);
}