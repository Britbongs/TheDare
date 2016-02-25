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
	~Player();
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
	bool getSprinting() const { return sprinting; }
	sf::RectangleShape getSprintRect() const { return sprintRect_; }
	sf::RectangleShape getHealthRect() const { return healthRect_; }
	sf::RectangleShape getHealthBox() const { return healthBox_; }
	sf::RectangleShape getSprintBox() const { return sprintBox_; }
	sf::FloatRect getPunchCollider() const { return punchCol_; }
	sf::Vector2f getMovementVector() const { return movement_; }
	const Animation& getPlayerWalkAnimation() const { return playerWalk_; }
	const Animation& getPlayerPunchAnimation() const { return playerPunch_; }

	void setAlive(const bool state) { alive = state; }
	void setCanTakeDamage(const bool state) { canTakeDamage = state; }
	void setAnimationState(const int state) { animationState = state; }

	void sprint();	
	void walk();	
	void punch();
	void update(const sf::Time&, const sf::Vector2f, const sf::RenderTexture*);
	void reset();
	void punchTimer();
	void pickupHealth(int);
	void takeDamage(const float damage);
	void shoot();
	void spritesheetSwitch(const int);

	bool invincibility();

	sf::RectangleShape colShape_;

private:
	sf::Texture spritesheet_;
	Animation playerWalk_;
	Animation playerWalkGun_;
	Animation playerDeath_;
	Animation playerPunch_;
	Animation playerShoot_;
	sf::RectangleShape sprintRect_;
	sf::RectangleShape healthRect_;
	sf::RectangleShape sprintBox_;
	sf::RectangleShape healthBox_;
	sf::Texture sprintTexture_;
	sf::Texture healthTexture_;
	sf::FloatRect punchCol_;
	sf::Clock invincClock_;
	sf::Clock punchClock_;
	sf::Time invincTimer_;
	sf::Time punchTimer_;
	sf::Vector2f rotationVector_;
	sf::Vector2f movement_;

	AudioManager* aManage_;

	sf::Sound footsteps_;
	sf::Sound hurtSnd_;
	sf::Sound deathSnd_;

	int animationState; //0 for walking 1 for punching

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
	bool sprinting;

	bool stepSoundPlaying;
	bool dying;
	bool dyingAnimDone;



private:

	bool initSpritesheet();
	bool initAudio();

	void updateMovement(const sf::Time&);
	void updateRotation(const sf::Vector2f);
	//void updateMelee(const sf::Vector2f);
	void updateSprintBar(const sf::RenderTexture*);
	void updateHealthBar(const sf::RenderTexture*);
	void getDirection(sf::Vector2f&);
	void normalizeMovement(sf::Vector2f&);
	
};
#endif