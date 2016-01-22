#ifndef SPAWNER_H
#define SPAWNER_H
#include <vector> 
#include "..\Entities\Enemy.h"
#include "..\Map\TiledMap.h"
#include "..\Utils\VectorUtils.h"
class Spawner
{
public:
	//Param: enemy count, position, vector of enemies, tiledmap pointer
	Spawner(int, sf::Vector2i, vector<Enemy>*, const TiledMap* const);
	void spawnEnemies();
private:
	void placeEnemy(Enemy&);
	void generateFreeLocations();
	const int enemyCount_;
	const sf::Vector2i spawnerPosition_;
	const TiledMap* const map_;
	enum POSITIONS{ UP_LEFT = 0, UP = 1, UP_RIGHT = 2, RIGHT = 3, 
		DOWN_RIGHT = 4, DOWN = 5, DOWN_LEFT = 6, LEFT = 6 };
	vector<POSITIONS> locations_;
	vector<Enemy>* const enemies_;
};
#endif