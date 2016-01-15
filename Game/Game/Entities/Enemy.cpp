#include "Enemy.h"
Enemy::Enemy()
	: alive(true), state(0), moveSpeed(250), maxHealth(100), currentHealth(100), damage(25)
{
	for (int i(0); i < this->getVertexCount(); ++i)
		this->setVertexColour(i, sf::Color::Green);
	setOrigin(0, 0);
	
	collider_.width = 60;
	collider_.height = 60;
	collider_.left = getPosition().x;
	collider_.top = getPosition().y;

	chaseBox_.width = 1024;
	chaseBox_.height = 1024;
	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 8);
	chaseBox_.top = getPosition().y - (getGlobalBounds().width * 8);

	healthRect_.setFillColor(sf::Color::Green); //init health rect with colour green
	healthRect_.setSize(sf::Vector2f(64, 5)); //init health rect with width of enemy and size of 5
	healthRect_.setPosition(getPosition().x, getPosition().y);

	//enemySprite_.loadFromFile("")
	
}

void Enemy::update(const sf::Time& delta, const sf::Vector2f& playerPos, const float rot)
{
	collider_.left = getPosition().x - (getGlobalBounds().width / 2);
	collider_.top = getPosition().y - (getGlobalBounds().height / 2);

	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 2);
	chaseBox_.top = getPosition().y - (getGlobalBounds().height * 2);

	healthRect_.setPosition(getPosition().x - (getGlobalBounds().width / 2), getPosition().y - (getGlobalBounds().height / 2) - 10);
	updateHealthBar();

	if (state == 1 && alive)
	{
		chase(delta, playerPos);
		updateRotation(rot);
	}
}

void Enemy::chase(const sf::Time& delta, const sf::Vector2f& playerPos)
{
	sf::Vector2f direction(0, 0);
	sf::Vector2f movement(0, 0);
	sf::Vector2f pos(getPosition().x + getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2);

	if (playerPos.x - pos.x < 0)
	{
		direction.x = -1;
	}
	if (playerPos.x - pos.x > 0)
	{
		direction.x = 1;
	}
	if (playerPos.y - pos.y < 0)
	{
		direction.y = -1;
	}
	if (playerPos.y - pos.y > 0)
	{
		direction.y = 1;
	}

	//create a vector that uses the two colliders and the direction to work out collisions
	sf::Vector2f a(direction.x * (delta.asSeconds() * moveSpeed), direction.y * (delta.asSeconds() * moveSpeed));


	movement = (p_tileMap_->getCollisionVector(collider_, a));

	if (movement.x != 0 && movement.y != 0) //if the movement vector is not (0,0)
	{
		sf::Vector2f normalized(normalize(movement));
		movement.x *= fabs(normalized.x);
		movement.y *= fabs(normalized.y);
	}

	move(movement);	//move the enemy
}

void Enemy::updateRotation(const float rot)
{
	setRotation(rot);
}

void Enemy::takeDamage(const float damage)
{
	currentHealth -= damage;
	std::cout << currentHealth << std::endl;
}

void Enemy::updateHealthBar()
{
	float scaleX = currentHealth / maxHealth; //get percentage of sprint timer
	healthRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Enemy::kill()
{
	alive = false;
	setPosition(512, 512);
}
