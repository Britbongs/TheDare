#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"	
#include "Bullet.h"
#include "..\Constants\Constants.h"

class Player
	: public Entity
{
public:
	Player();
	bool init();

	float getMoveSpeed() const { return moveSpeed; }
	float getMaxSprint() const { return maxSprint; }
	float getSprintTime() const { return sprintTime; }
	float getCurrentHealth() const { return currentHealth; }
	float getInvincibleTime() const { return invincTime;  }
	bool getAlive() const { return alive;  }
	bool getCanTakeDamage() const { return canTakeDamage;  }
	sf::RectangleShape getSprintRect() const { return sprintRect_; }
	sf::RectangleShape getHealthRect() const { return healthRect_; }

	void setAlive(const bool state) { alive = state; }
	void setCanTakeDamage(const bool state) { canTakeDamage = state; }
	void takeDamage(const float damage){ currentHealth -= damage; }

	void sprint();
	void walk();
	void update(const sf::Time&, const float, const sf::RenderTexture*);
	void resetHealth();

	bool invincibility();

private:
	sf::Texture spritesheet_;
	Animation playerWalk_;
	sf::RectangleShape sprintRect_;
	sf::RectangleShape healthRect_;
	sf::Clock invincClock_;
	sf::Time invincTimer_;

	float moveSpeed;
	float maxSprint;
	float maxHealth, currentHealth;
	float sprintTime;
	float invincTime;

	bool alive;
	bool clockStarted;
	bool canTakeDamage;

private:

	bool initSpritesheet();

	void updateMovement(const sf::Time&);
	void updateRotation(const float);
	void updateSprintBar(const sf::RenderTexture*);
	void updateHealthBar(const sf::RenderTexture*);
	void getDirection(sf::Vector2f&);
	void normalizeMovement(sf::Vector2f&);
	
};
#endif