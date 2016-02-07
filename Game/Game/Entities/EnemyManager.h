#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>
#include <assert.h>
#include <SFML\Graphics.hpp>


#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"

#include "..\Constants\Constants.h"
#include "..\Map\TiledMap.h"

class EnemyManager
{
public:
	//Public functions
	EnemyManager(sf::RenderTexture*, Player*, Bullet*, TiledMap*);
	~EnemyManager();
	static EnemyManager* Get(); //returns a pointer to the EnemyManager
	bool init();
	void update(const sf::Time&);
	void draw() const;
	Enemy* getEnemy(int)const;
	int getEnemyCount() const { return enemies_.size(); }
	void reset();
private:
	//Private functions 
	void handleRotation(Enemy*, const sf::Vector2f&);
	void handleBulletCollision(Enemy*);
	void handleCombat(Enemy*);
private:
	//Private data members
	typedef std::vector<Enemy*> EnemyList;
	EnemyList enemies_;
	static EnemyManager* eManager_;

	sf::RenderTexture* renderTexture_;
	sf::Texture txtSpriteSheet_; //Spritesheet texture
	
	Bullet* bullets_;
	Player* player_;
	TiledMap* map_;
	
};
#endif
