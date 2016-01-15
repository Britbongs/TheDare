#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
class Enemy
	: public Entity
{
public:
	Enemy();
	
	float getMoveSpeed() const { return moveSpeed; }
	float getDamage() const { return damage; }
	float getCurrentHealth() const { return currentHealth;  }
	sf::RectangleShape getHealthRect() const { return healthRect_; }

	bool init();
	void update(const sf::Time&, const sf::Vector2f&, const float);
	void chase(const sf::Time&,const sf::Vector2f&);
	void takeDamage(const float);
	void kill();
	void resetHealth();

	sf::FloatRect chaseBox_;
	int state;
	
private:
	float moveSpeed;
	float maxHealth, currentHealth;
	float damage;
	bool alive;

	sf::Texture spritesheet_;
	Animation enemyWalk_;
	
	sf::RectangleShape healthRect_;
	sf::Texture enemySprite_;
private:

	void updateMovement(const sf::Time&);
	void updateRotation(const float);
	void updateHealthBar();
	sf::Vector2f isCollision(const sf::FloatRect&, const sf::Vector2f&, Player*);
};
#endif