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
	float getPunchDamage() const { return punchDamage; }
	bool getAlive() const { return alive;  }
	bool getCanTakeDamage() const { return canTakeDamage;  }
	bool getCanPunch() const { return canPunch; }
	sf::RectangleShape getSprintRect() const { return sprintRect_; }
	sf::RectangleShape getHealthRect() const { return healthRect_; }
	sf::FloatRect getPunchCollider() const { return punchCol_; }

	void setAlive(const bool state) { alive = state; }
	void setCanTakeDamage(const bool state) { canTakeDamage = state; }
	void takeDamage(const float damage){ currentHealth -= damage; }

	void sprint();
	void walk();
	void punch();
	void update(const sf::Time&, const sf::Vector2f, const sf::RenderTexture*);
	void resetHealth();
	void punchTimer();

	bool invincibility();

private:
	sf::Texture spritesheet_;
	Animation playerWalk_;
	sf::RectangleShape sprintRect_;
	sf::RectangleShape healthRect_;
	sf::FloatRect punchCol_;
	sf::Clock invincClock_;
	sf::Clock punchClock_;
	sf::Time invincTimer_;
	sf::Time punchTimer_;
	sf::Vector2f rotationVector_;

	float moveSpeed;
	float maxSprint;
	float maxHealth, currentHealth;
	float sprintTime;
	float invincTime;
	float punchTime;
	float punchRange;
	float punchDamage;

	bool alive;
	bool invinClockStarted;
	bool punchClockStarted;
	bool canPunch;
	bool canTakeDamage;

private:

	bool initSpritesheet();

	void updateMovement(const sf::Time&);
	void updateRotation(const sf::Vector2f);
	void updateMelee(const sf::Vector2f);
	void updateSprintBar(const sf::RenderTexture*);
	void updateHealthBar(const sf::RenderTexture*);
	void getDirection(sf::Vector2f&);
	void normalizeMovement(sf::Vector2f&);
	
};
#endif