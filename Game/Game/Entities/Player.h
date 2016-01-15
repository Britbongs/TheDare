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

	void setAlive(bool);
	void setCanTakeDamage(bool);

	void sprint();
	void walk();
	void takeDamage(const float);
	bool invincibility();
	void update(const sf::Time&, const float, const sf::RenderTexture*);
	void resetHealth();

private:
	sf::Texture spritesheet_;
	Animation playerWalk_;
	float moveSpeed;
	float maxSprint;
	float maxHealth, currentHealth;
	float sprintTime;
	float invincTime;
	bool alive;
	bool clockStarted;
	bool canTakeDamage;
	sf::RectangleShape sprintRect_;
	sf::RectangleShape healthRect_;
	sf::Clock invincClock;
	sf::Time invincTimer;

private:

	void updateMovement(const sf::Time&);
	void updateRotation(const float);
	void updateSprintBar(const sf::RenderTexture*);
	void updateHealthBar(const sf::RenderTexture*);
	
};
#endif