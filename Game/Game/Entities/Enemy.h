#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
class Enemy
	: public Entity
{
public:
	Enemy();
	bool init();
	
	float getMoveSpeed() const { return moveSpeed; }
	float getDamage() const { return damage; }
	float getCurrentHealth() const { return currentHealth;  }
	sf::RectangleShape getHealthRect() const { return healthRect_; }
	sf::FloatRect getChaseBox() const { return chaseBox_; }
	int getState() const { return state; }
	
	void takeDamage(const float damage) { currentHealth -= damage; }
	void resetHealth() { currentHealth = maxHealth; }
	void setState(const int newState) { state = newState;  }

	void update(const sf::Time&, const sf::Vector2f&, const float);
	void chase(const sf::Time&,const sf::Vector2f&);
	void kill();

	
private:

	sf::Texture spritesheet_;
	Animation enemyWalk_;
	sf::RectangleShape healthRect_;
	sf::Texture enemySprite_;
	sf::FloatRect chaseBox_;

	int state;
	
	float moveSpeed;
	float maxHealth, currentHealth;
	float damage;

	bool alive;

private:
	void updateMovement(const sf::Time&);
	void updateRotation(const float);
	void updateHealthBar();
	sf::Vector2f isCollision(const sf::FloatRect&, const sf::Vector2f&, Player*);
};
#endif