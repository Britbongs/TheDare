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
	const sf::RectangleShape getChersBox() const { return chaseBerx_; }

	void takeDamage(const float damage) { currentHealth -= damage; }
	void resetHealth() { currentHealth = maxHealth; }
	void setChasing(bool chase) { chase == true ? state_ = State::CHASING : state_ = State::PATROL; }
	void setCanTakeDamage(const bool state) { canTakeDamage = state; }

	void update(const sf::Time&, const sf::Vector2f&);
	void chase(const sf::Time&, const sf::Vector2f&);
	void kill();

	bool invincibility();
	bool isChasing() const { return (state_ == State::CHASING); }
	sf::RectangleShape colliderShape_;
	bool collidedX_;
	bool collidedY_;

private:

	enum State
	{
		PATROL = 0,
		CHASING = 1,
		DEAD = 2
	};

	sf::Texture spritesheet_;
	Animation enemyWalk_;
	sf::RectangleShape healthRect_;
	sf::RectangleShape chaseBerx_;
	sf::Texture enemySprite_;
	sf::FloatRect chaseBox_;
	sf::Clock invincClock_;
	sf::Time invincTimer_;
	sf::Vector2f movementVector_;
	sf::Vector2i* target_;

	int state;
	int pathIndex_;

	State state_;

	float moveSpeed;
	float maxHealth, currentHealth;
	float damage;
	float invincTime;

	bool canTakeDamage;
	bool invinClockStarted;


private:
	vector<sf::Vector2i> path_;
	bool initSpritesheet();
	void updateHealthBar();
	void generatePath(const sf::Vector2i&, const sf::Vector2i&);
	void walkToNextPosition(const sf::Time&); //For A*
};
#endif