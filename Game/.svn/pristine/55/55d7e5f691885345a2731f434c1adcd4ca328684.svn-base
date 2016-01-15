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

	void update(const sf::Time&, const sf::Vector2f&, const float);
	void chase(const sf::Time&,const sf::Vector2f&);
	void takeDamage(const float);
	void kill();

	sf::FloatRect chaseBox_;
	int state;
	
private:
	float moveSpeed;
	float maxHealth, currentHealth;
	float damage;
	bool alive;
	
	sf::RectangleShape healthRect_;
	sf::Texture enemySprite_;
private:

	void updateMovement(const sf::Time&);
	void updateRotation(const float);
	void updateHealthBar();
};
#endif