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
	bool getCanTakeDamage() const { return canTakeDamage; }
	sf::RectangleShape getHealthRect() const { return healthRect_; }
	sf::FloatRect getChaseBox() const { return chaseBox_; }
	sf::Vector2f getMovementVector() const { return movementVector_; }
	int getState() const { return state; }
	
	void takeDamage(const float damage) { currentHealth -= damage; }
	void resetHealth() { currentHealth = maxHealth; }
	void setState(const int newState) { state = newState;  }
	void setCanTakeDamage(const bool state) { canTakeDamage = state; }

	void update(const sf::Time&, const sf::Vector2f&, const float);
	void chase(const sf::Time&,const sf::Vector2f&);
	void kill();

	bool invincibility();

	sf::RectangleShape colliderShape_;
	bool collidedX_;
	bool collidedY_;

private:

	sf::Texture spritesheet_;
	Animation enemyWalk_;
	sf::RectangleShape healthRect_;
	sf::Texture enemySprite_;
	sf::FloatRect chaseBox_;
	sf::Clock invincClock_;
	sf::Time invincTimer_;
	sf::Vector2f movementVector_;
	int state;
	
	float moveSpeed;
	float maxHealth, currentHealth;
	float damage;
	float invincTime;

	bool alive;
	bool canTakeDamage;
	bool invinClockStarted;

private:
	
	bool initSpritesheet();
	void updateRotation(const float);
	void updateHealthBar();
};
#endif