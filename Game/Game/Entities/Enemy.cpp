#include "Enemy.h"
Enemy::Enemy()
	: state(0), moveSpeed(250), maxHealth(100), currentHealth(100), damage(25), invinClockStarted(false), canTakeDamage(true), invincTime(0.5f),
	collidedX_(false), collidedY_(false)
{
	setAlive(false);
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

	//audio
	hurtSnd_.setBuffer("enemyHurt.ogg");
	hurtSnd_.setSoundToBuffer();
	hurtSnd_.sound_.setLoop(0);
	hurtSnd_.sound_.setVolume(25);

	deathSnd_.setBuffer("enemyDeath.ogg");
	deathSnd_.setSoundToBuffer();
	deathSnd_.sound_.setLoop(0);
	deathSnd_.sound_.setVolume(40);

	if (!initSpritesheet())
		return(false);

	return(true);
}

bool Enemy::initSpritesheet()
{
	/*
	if (!spritesheet_.loadFromFile("res//entities//enemyspritesheet.png"))
		return(false);
		*/
	isAnimated(true);

	//enemyWalk_.setSpriteSheet(spritesheet_);
	enemyWalk_.setSpriteSheet(*getTexture());
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


void Enemy::update(const sf::Time& delta, const sf::Vector2f& playerPos)
{

	collider_.left = getPosition().x - (collider_.width / 2);// - (getGlobalBounds().width / 2);
	collider_.top = getPosition().y - (collider_.height / 2);// -(getGlobalBounds().height / 2);

	colliderShape_.setPosition(collider_.left, collider_.top);

	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 2);
	chaseBox_.top = getPosition().y - (getGlobalBounds().height * 2);

	healthRect_.setPosition(getPosition().x - (getGlobalBounds().width / 2), getPosition().y - (getGlobalBounds().height / 2) - 10);
	updateHealthBar();
	/*
	if (state == 0)
	{
		setFrame(0);
	}
	if (state == 1 && alive)
	{
		updateAnimation(delta);
		chase(delta, playerPos);
		updateRotation(rot);
	}*/

	switch (state_)
	{
	case State::PATROL:
		//setFrame(0); 
		updateAnimation(delta);
		break;
	case State::CHASING:
		if (getAlive())
		{
			updateAnimation(delta);
			chase(delta, playerPos);
		}
		break;
	case State::DEAD:
		setFrame(0);
		break;
	}
}

//void Enemy::chase(const sf::Time& delta, const sf::Vector2f& playerPos)
//{
//	sf::Vector2f direction(0, 0);
//	sf::Vector2f movement(0, 0);
//	//currentVelocity_ = desiredVelocity_;
//	sf::Vector2f pos(getPosition().x + getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2);
//	if (!collidedX_ && !collidedY_)
//	{
//
//		if (playerPos.x - pos.x < 0)
//		{
//			direction.x = -0.25f;
//		}
//		if (playerPos.x - pos.x > 0)
//		{
//			direction.x = 0.25f;
//		}
//		if (playerPos.y - pos.y < 0)
//		{
//			direction.y = -0.25f;
//		}
//		if (playerPos.y - pos.y > 0)
//		{
//			direction.y = 0.25f;
//		}
//	}
//	//else
//	//{
//	//	direction.x = 0;
//	//	direction.y = 0;
//	//}
//	//create a vector that uses the two colliders and the direction to work out collisions
//	sf::Vector2f a(direction.x * (delta.asSeconds() * moveSpeed), direction.y * (delta.asSeconds() * moveSpeed));
//
//
//	movementVector_ = movement;
//	movement = (p_tileMap_->getCollisionVector(collider_, a, getID()));
//
//	if (movement.x != 0 && movement.y != 0) //if the movement vector is not (0,0)
//	{
//		sf::Vector2f normalized(normalize(movement));
//		movement.x *= fabs(normalized.x);
//		movement.y *= fabs(normalized.y);
//	}
//
//
//	move(movement);	//move the enemy
//}


void Enemy::chase(const sf::Time& delta, const sf::Vector2f& playerPos)
{
	if (path_.size() > 0)
	{
		const sf::Vector2f end(static_cast<float> (path_.back().x), static_cast<float> (path_.back().y));
		if (getVectorLength(subtractVector(playerPos, end)))
		{

		}
	}
	else
	{
		sf::Vector2i gridPos(static_cast<int> (getPosition().x / gconsts::Gameplay::TILESIZE), static_cast<int> (getPosition().y / gconsts::Gameplay::TILESIZE));
		sf::Vector2i playerGridPos(static_cast<int> (floor(playerPos.x / 64.f)), static_cast<int> (floorf(playerPos.y / 64.f)));
		path_ = aStarPath(gridPos, playerGridPos, *p_tileMap_);

	}
}


void Enemy::updateHealthBar()
{
	float scaleX = currentHealth / maxHealth; //get percentage of sprint timer
	healthRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Enemy::kill()
{
	deathSnd_.sound_.play();
	setAlive(false);
	state = DEAD;
	setPosition(512, 512);
}

void Enemy::takeDamage(const float damage)
{
	currentHealth -= damage;
	hurtSnd_.sound_.play();
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