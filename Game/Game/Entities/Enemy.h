#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

#include "..\Utils\Utils.h"
class Enemy
	: public Entity
{
public:
	Enemy();
	bool init();

	float getMoveSpeed() const { return moveSpeed; }
	float getDamage() const { return damage; }
	float getCurrentHealth() const { return currentHealth; }
	bool getCanTakeDamage() const { return canTakeDamage; }
	sf::RectangleShape getHealthRect() const { return healthRect_; }
	sf::FloatRect getChaseBox() const { return chaseBox_; }
	sf::Vector2f getMovementVector() const { return movementVector_; }

	void resetHealth() { currentHealth = maxHealth; }
	void setChasing(bool chase) { chase ? state_ = State::CHASING : state_ == State::PATROL; }
	void setCanTakeDamage(const bool state) { canTakeDamage = state; }

	void update(const sf::Time&, const sf::Vector2f&);
	void chase(const sf::Time&, const sf::Vector2f&);
	void kill();
	void takeDamage(const float damage);

	bool invincibility();
	bool isChasing() const { return (state_ == State::CHASING); }
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

	Audio hurtSnd_;
	Audio deathSnd_;

	int state;

	float moveSpeed;
	float maxHealth, currentHealth;
	float damage;
	float invincTime;

	bool canTakeDamage;
	bool invinClockStarted;


private:

	enum State
	{
		PATROL = 0,
		CHASING = 1,
		DEAD = 2
	};

	vector<sf::Vector2i> path_;
	bool initSpritesheet();
	void updateHealthBar();
	State state_;
};
#endif