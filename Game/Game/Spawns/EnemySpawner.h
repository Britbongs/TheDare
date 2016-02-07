#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <vector>

#include "..\Entities\Enemy.h"
#include "..\Entities\EnemyManager.h"

class EnemySpawner
{
public:
	EnemySpawner(int, sf::Vector2i, const TiledMap* const);
	EnemySpawner(int, sf::Vector2i, const TiledMap* const, bool, int);

	void spawnEnemies();
	void reset();

	int getSpawnerID() const;

	bool isTriggeredSpawner() const;
	bool hasBeenTriggered() const;
private:
	void placeEnemy(Enemy*);
	void generateFreeLocations();
private: 
	const int enemyCount_; 
	const bool isTriggeredSpawn_; 
	const sf::Vector2i position_; 
	const TiledMap* const map_; 
	const int trigID_; 
	enum POSITIONS {
		UP_LEFT = 0, UP = 1, UP_RIGHT = 2, RIGHT = 3,
		DOWN_RIGHT = 4, DOWN = 5, DOWN_LEFT = 6, LEFT = 7, CENTRE = 8
	};
	std::vector<POSITIONS> locations_;
	std::vector<Enemy*> enemies_;
	bool triggered_;

};
#endif
