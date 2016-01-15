#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"	

class Bullet
	: public Entity
{
public:
	Bullet();

	float getDamage() const { return damage; }

	void update(const sf::Time&);
	void init(const sf::Vector2f&, const sf::Vector2f&);

private:
	float rotation;
	float moveSpeed;
	float damage;
	bool ready, alive;
	sf::Vector2f direction;
private:
	void updateMovement(const sf::Time&);
};

#endif