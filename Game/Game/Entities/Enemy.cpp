#include "Enemy.h"
Enemy::Enemy()
	: alive(true), state(0), moveSpeed(250), maxHealth(100), currentHealth(100), damage(25), invinClockStarted(false), canTakeDamage(true), invincTime(0.5f),
	collidedX_(false), collidedY_(false)
{
}

bool Enemy::init()
{

	setOrigin(0, 0);
	collider_.width = 55;
	collider_.height = 55;
	collider_.left = getPosition().x;
	collider_.top = getPosition().y;

	colliderShape_.setSize(sf::Vector2f(55, 55));
	colliderShape_.setPosition(collider_.left, collider_.top);
	colliderShape_.setFillColor(sf::Color::Red);

	chaseBox_.width = 1024;
	chaseBox_.height = 1024;
	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 8);
	chaseBox_.top = getPosition().y - (getGlobalBounds().width * 8);

	healthRect_.setFillColor(sf::Color::Green); //init health rect with colour green
	healthRect_.setSize(sf::Vector2f(64, 5)); //init health rect with width of enemy and size of 5
	healthRect_.setPosition(getPosition().x, getPosition().y);

	if (!initSpritesheet())
		return(false);

	return(true);
}

bool Enemy::initSpritesheet()
{

	if (!spritesheet_.loadFromFile("res//entities//enemyspritesheet.png"))
		return(false);

	isAnimated(true);

	enemyWalk_.setSpriteSheet(spritesheet_);

	const int SIZE(128);

	enemyWalk_.addFrame(sf::IntRect(0 * SIZE, 0 * SIZE, SIZE, SIZE));
	enemyWalk_.addFrame(sf::IntRect(1 * SIZE, 0 * SIZE, SIZE, SIZE));
	enemyWalk_.addFrame(sf::IntRect(2 * SIZE, 0 * SIZE, SIZE, SIZE));

	setAnimation(enemyWalk_);
	setAnimationLoop(true);
	playAnimation();
	sf::Time frameTime = sf::milliseconds(150);
	setFrameTime(frameTime);

	return(true);
}


void Enemy::update(const sf::Time& delta, const sf::Vector2f& playerPos, const float rot)
{

	collider_.left = getPosition().x - (collider_.width / 2);// - (getGlobalBounds().width / 2);
	collider_.top = getPosition().y - (collider_.height / 2);// -(getGlobalBounds().height / 2);

	colliderShape_.setPosition(collider_.left, collider_.top);

	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 2);
	chaseBox_.top = getPosition().y - (getGlobalBounds().height * 2);

	healthRect_.setPosition(getPosition().x - (getGlobalBounds().width / 2), getPosition().y - (getGlobalBounds().height / 2) - 10);
	updateHealthBar();

	if (state == 0)
	{
		setFrame(0);
	}
	if (state == 1 && alive)
	{
		updateAnimation(delta);
		chase(delta, playerPos);
		updateRotation(rot);
	}
}

void Enemy::chase(const sf::Time& delta, const sf::Vector2f& playerPos)
{
	sf::Vector2f direction(0, 0);
	sf::Vector2f movement(0, 0);
	//currentVelocity_ = desiredVelocity_;
	sf::Vector2f pos(getPosition().x + getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2);
	if (!collidedX_ && !collidedY_)
	{

		if (playerPos.x - pos.x < 0)
		{
			direction.x = -0.25f;
		}
		if (playerPos.x - pos.x > 0)
		{
			direction.x = 0.25f;
		}
		if (playerPos.y - pos.y < 0)
		{
			direction.y = -0.25f;
		}
		if (playerPos.y - pos.y > 0)
		{
			direction.y = 0.25f;
		}
	}
	//else
	//{
	//	direction.x = 0;
	//	direction.y = 0;
	//}
//create a vector that uses the two colliders and the direction to work out collisions
sf::Vector2f a(direction.x * (delta.asSeconds() * moveSpeed), direction.y * (delta.asSeconds() * moveSpeed));


movementVector_ = movement;
movement = (p_tileMap_->getCollisionVector(collider_, a, getID()));

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

bool Enemy::invincibility()
{
	if (canTakeDamage == false)
	{
		if (invinClockStarted == false)
		{
			invinClockStarted = true;

			invincClock_.restart();
		}
		invincTimer_ = invincClock_.getElapsedTime();
		if (invincTimer_.asSeconds() >= invincTime)
		{
			canTakeDamage = true;
			invinClockStarted = false;
			return true;
		}
	}
	return false;
}