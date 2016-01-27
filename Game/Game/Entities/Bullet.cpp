#include "Bullet.h"

Bullet::Bullet()
	: rotation(0), moveSpeed(60.f), ready(true), alive(false), damage(50)

{
}

void Bullet::init(const sf::Vector2f& rotation, const sf::Vector2f& position)
{
	setOrigin(0.5f, 0.5f);
	collider_.width = 10;
	collider_.height = 5;

	float rot = (degrees(atan2(rotation.y, rotation.x)));
	float randAccuracy = random(-10, 10);
	rot += randAccuracy;
	setPosition(position);
	setRotation(rot);
	direction = rotation;

	if (direction.x > 0)
	{
		direction.x += randAccuracy;
	}
	else
	{
		direction.x -= randAccuracy;
	}
	if (direction.y > 0)
	{
		direction.y += randAccuracy;
	}
	else
	{
		direction.y -= randAccuracy;
	}

	if (direction.x != 0 && direction.y != 0) //if the movement vector is not (0,0)
	{

		direction = sf::Vector2f(normalize(direction)); //normalize the movement vector so diagnal movement isn't quicker
	}
	else
	{
		if (direction.x == 0)
		{
			direction.y = direction.y / fabs(direction.y);
		}
		if (direction.y == 0)
		{
			direction.x = direction.x / fabs(direction.x);
		}
	}
}

void Bullet::update(const sf::Time& delta)
{
	if (getAlive())
	{
		updateMovement(delta);
	}
}

void Bullet::updateMovement(const sf::Time& delta)
{
	sf::Vector2f mov;
	mov = direction;
	mov.x *= moveSpeed * delta.asSeconds();	//times the movement vector by moveSpeed and delta
	mov.y *= moveSpeed * delta.asSeconds();

	collider_.left = getPosition().x;
	collider_.top = getPosition().y;
	if (!p_tileMap_->isCollided(collider_, mov))
	{
		move(mov);	//move the bullet
	}
	else
	{
		setAlive(false);
	}

	//std::cout << alive << std::endl;

}
