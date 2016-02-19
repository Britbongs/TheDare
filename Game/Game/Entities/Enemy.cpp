#include "Enemy.h"
Enemy::Enemy()
	: state(0), moveSpeed(190), maxHealth(100), currentHealth(100), damage(25), invinClockStarted(false), canTakeDamage(true), invincTime(0.5f),
	collidedX_(false), collidedY_(false)
{
	setAlive(false);
}

Enemy::~Enemy()
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

	chaseBox_.width = 256;
	chaseBox_.height = 256;
	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 8);
	chaseBox_.top = getPosition().y - (getGlobalBounds().width * 8);

	chaseBerx_.setSize(sf::Vector2f(256, 256));
	chaseBerx_.setFillColor(sf::Color::Green);
	healthRect_.setFillColor(sf::Color::Green); //init health rect with colour green
	healthRect_.setSize(sf::Vector2f(64, 5)); //init health rect with width of enemy and size of 5
	healthRect_.setPosition(getPosition().x, getPosition().y);

	aManage_ = AudioManager::Get();

	if (!initAudio())
		return(false);

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

bool Enemy::initAudio()
{
	hurtSnd_.setBuffer(aManage_->buffers_[aManage_->ENEMY_HURT]);
	hurtSnd_.setLoop(0);
	hurtSnd_.setVolume(25);

	deathSnd_.setBuffer(aManage_->buffers_[aManage_->ENEMY_DEATH]);
	deathSnd_.setLoop(0);
	deathSnd_.setVolume(30);
	deathSnd_.setPitch(1.5f);

	return(true);
}


void Enemy::update(const sf::Time& delta, const sf::Vector2f& playerPos)
{

	collider_.left = getPosition().x - (collider_.width / 2);// - (getGlobalBounds().width / 2);
	collider_.top = getPosition().y - (collider_.height / 2);// -(getGlobalBounds().height / 2);

	colliderShape_.setPosition(collider_.left, collider_.top);

	chaseBox_.left = getPosition().x - (getGlobalBounds().width * 2);
	chaseBox_.top = getPosition().y - (getGlobalBounds().height * 2);
	chaseBerx_.setPosition(chaseBox_.left, chaseBox_.top);
	healthRect_.setPosition(getPosition().x - (getGlobalBounds().width / 2), getPosition().y - (getGlobalBounds().height / 2) - 10);

	if (!canTakeDamage)
	{
		if (invincibility())
		{
			canTakeDamage = true;
		}
	}
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

void Enemy::dumbChase(const sf::Time& delta, const sf::Vector2f& playerPos)
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
	else
	{/* 		
	 if (movement.x != 0)
			movement.x /= abs(movement.x);
		if (movement.y != 0)
			movement.y /= abs(movement.y);

		movement.x *= fabs(movement.x);
		movement.y *= fabs(normalized.y);
		*/
	}


	move(movement);	//move the enemy
}


void Enemy::chase(const sf::Time& delta, const sf::Vector2f& playerPos)
{
	const sf::Vector2i gridPos(static_cast<int> (getPosition().x / gconsts::Gameplay::TILESIZE), static_cast<int> (getPosition().y / gconsts::Gameplay::TILESIZE));
	const sf::Vector2i playerGridPos(static_cast<int> (floor((playerPos.x + gconsts::Gameplay::HALF_TILESIZE) / gconsts::Gameplay::TILESIZE) - 1),
		static_cast<int> (floor((playerPos.y + gconsts::Gameplay::HALF_TILESIZE) / gconsts::Gameplay::TILESIZE)) - 1);
	float distToPlayer(getVectorLength(subtractVector(playerPos, getPosition())));
	if (distToPlayer > gconsts::Gameplay::TILESIZE * 8)
	{//If the player is out of a certain range, stop chasing. 
		setChasing(false); 
		return;
	}
	if (distToPlayer < 4 * gconsts::Gameplay::TILESIZE)
	{//If the player is really close to the enemy use the dumb chase cause it's more efficient
		dumbChase(delta, playerPos);
	}
	else
	{//otherwise, use the A* method
		//std::cout << playerGridPos.x << " - " << playerGridPos.y << " : " << gridPos.x << " - " << gridPos.y << std::endl;
		if (path_.size() > 0)
		{//if the enemy has a path

			const sf::Vector2f end(static_cast<float> (path_.back().x * gconsts::Gameplay::TILESIZE), static_cast<float> (path_.back().y * gconsts::Gameplay::TILESIZE));
			const sf::Vector2f current(static_cast<float>(path_[pathIndex_].x * gconsts::Gameplay::TILESIZE), static_cast<float> (path_[pathIndex_].y * gconsts::Gameplay::TILESIZE));
			const sf::Vector2f roundedPlayerPos(
				static_cast<float> (playerGridPos.x * gconsts::Gameplay::TILESIZE),
				static_cast<float> (playerGridPos.y * gconsts::Gameplay::TILESIZE));
			bool canWalk(true);
			float distance(getVectorLength(subtractVector(roundedPlayerPos, end)));

			//if (getVectorLength(subtractVector(playerGridPos, path_.back())) > 2.f)
			if (distance > gconsts::Gameplay::HALF_TILESIZE)
			{ //Is the player too far away from the end 
				generatePath(gridPos, playerGridPos);
				canWalk = false;
			}
			if (canWalk)
			{
				//if (getVectorLength(subtractVector(gridPos, path_[pathIndex_])) > 0.5f)
				if (getVectorLength(subtractVector(getPosition(), current)) > 8.f)
				{//if the enemy is greater than a distance of 1 from a tile
					walkToNextPosition(delta);
				}
				else
				{
					if ((pathIndex_ + 1) < static_cast<int>  (path_.size()))
					{//if the next position in the path is allowed
						++pathIndex_;
						walkToNextPosition(delta);
					}
				}
			}


		}
		else
		{//If the enemy has no path 
			if (getVectorLength(subtractVector(playerPos, getPosition())) > 2.5f)
			{ //Is the player too far away from the end 
				generatePath(gridPos, playerGridPos);

			}
			//generatePath(gridPos, playerGridPos);
		}
	}
}

void Enemy::updateHealthBar()
{
	float scaleX = currentHealth / maxHealth; //get percentage of sprint timer
	healthRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Enemy::generatePath(const sf::Vector2i & start, const sf::Vector2i & end)
{
	pathIndex_ = 0;
	path_ = aStarPath(start, end, *p_tileMap_);
}

void Enemy::walkToNextPosition(const sf::Time& delta)
{
	std::cout << getOrigin().x << " - " << getOrigin().y << std::endl;
	sf::Vector2f direction;
	sf::Vector2f location(static_cast<float> (path_[pathIndex_].x * gconsts::Gameplay::TILESIZE),
		static_cast<float> (path_[pathIndex_].y * gconsts::Gameplay::TILESIZE));

	direction = subtractVector(location, getPosition());
	direction = sf::Vector2f(roundf(direction.x), roundf(direction.y));
	if (direction.x != 0.f && direction.y != 0.f)//if we won't divide by zero
		direction = normalize(direction);//normalise the vector
	else
	{
		if (direction.y != 0.f)
		{//if the y isn't 0, make it of unit length
			direction.y /= fabs(direction.y);
		}

		if (direction.x != 0.f)
		{//if the x isn't 0, make it of unit length
			direction.x /= fabs(direction.x);
		}
	}

	direction.x *= delta.asSeconds() * moveSpeed;
	direction.y *= delta.asSeconds() * moveSpeed;

	std::cout << direction.x << " - " << direction.y << std::endl;



	move(direction);
}

void Enemy::kill()
{
	deathSnd_.play();
	setAlive(false);
	state = DEAD;
	setPosition(512, 512);
}

void Enemy::takeDamage(const float damage)
{
	currentHealth -= damage;
	if (currentHealth > 0)
		hurtSnd_.play();
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
	return (false);
}