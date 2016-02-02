#include "Player.h"	
#include <iostream>
Player::Player()
	: moveSpeed(250), maxSprint(500), sprintTime(maxSprint), maxHealth(100), currentHealth(maxHealth), alive(true), invinClockStarted(false), canTakeDamage(true), invincTime(1.f),
	punchRange(30.f), canPunch(true), punchClockStarted(false), punchTime(0.3f), punchDamage(25.f), animationState(0), sprinting(false)
{
}
bool Player::init()
{
	if (!initSpritesheet())
		return(false);

	collider_.width = getGlobalBounds().width * 0.90;
	collider_.height = getGlobalBounds().height * 0.90;
	collider_.top = -64;
	collider_.left = -64;
	colShape_.setScale(collider_.width, collider_.height);
	colShape_.setSize(sf::Vector2f(1, 1));
	colShape_.setPosition(collider_.left, collider_.top);
	colShape_.setFillColor(sf::Color::Green);

	punchCol_.width = getGlobalBounds().width * 0.90;
	punchCol_.height = getGlobalBounds().height * 0.90;

	sprintRect_.setFillColor(sf::Color::Red); //init sprint rect with colour red
	sprintRect_.setSize(sf::Vector2f(64, 5)); //init sprint rect with width of player and size of 5

	healthRect_.setFillColor(sf::Color::Green); //init sprint rect with colour red
	healthRect_.setSize(sf::Vector2f(64, 5)); //init sprint rect with width of player and size of 5

	return(true);
}

bool Player::initSpritesheet()
{
	if (!spritesheet_.loadFromFile("res//entities//spritesheet.png"))
		return(false);

	isAnimated(true);

	playerWalk_.setSpriteSheet(spritesheet_);

	const int SIZE(128);

	playerWalk_.addFrame(sf::IntRect(0 * SIZE, 2 * SIZE, SIZE, SIZE));
	playerWalk_.addFrame(sf::IntRect(1 * SIZE, 2 * SIZE, SIZE, SIZE));
	playerWalk_.addFrame(sf::IntRect(2 * SIZE, 2 * SIZE, SIZE, SIZE));
	playerWalk_.addFrame(sf::IntRect(3 * SIZE, 2 * SIZE, SIZE, SIZE));

	setAnimationLoop(true);
	playAnimation();
	sf::Time frameTime = sf::milliseconds(150);
	setFrameTime(frameTime);
	setAnimation(playerWalk_);

	playerPunch_.setSpriteSheet(spritesheet_);
	playerPunch_.addFrame(sf::IntRect(0 * SIZE, 1 * SIZE, SIZE, SIZE));
	playerPunch_.addFrame(sf::IntRect(1 * SIZE, 1 * SIZE, SIZE, SIZE));
	playerPunch_.addFrame(sf::IntRect(2 * SIZE, 1 * SIZE, SIZE, SIZE));
	playerPunch_.addFrame(sf::IntRect(3 * SIZE, 1 * SIZE, SIZE, SIZE));

	return true;
}

void Player::update(const sf::Time& delta, const sf::Vector2f rotVec, const sf::RenderTexture* renderTexture)
{
	colShape_.setPosition(collider_.left, collider_.top);
	updateMovement(delta);
	updateSprintBar(renderTexture);
	updateHealthBar(renderTexture);
	updateRotation(rotVec);
	updateAnimation(delta);
}

void Player::sprint()
{
	if (sprintTime > 0) //if the sprint timer is greater then 0 then allow sprinting
	{
		sprinting = true;
		moveSpeed = 1000;
		--sprintTime; //decrease sprint timer towards 0
	}
	else
	{
		moveSpeed = 200; //sprint timer is equal or less than 0 change move speed to walking pace
	}

}

void Player::walk()
{
	sprinting = false;
	moveSpeed = 200; //make sure move speed is walking pace
	if (sprintTime < maxSprint) //if sprint timer is less than the max sprint duration then
	{
		sprintTime += 0.5f; //increase sprint timer back up towards the max duration
	}
}

void Player::updateSprintBar(const sf::RenderTexture* renderTexture)
{
	float scaleX = sprintTime / maxSprint; //get percentage of sprint timer
	sprintRect_.setPosition(renderTexture->mapPixelToCoords(sf::Vector2i(10, 100)));
	sprintRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Player::updateHealthBar(const sf::RenderTexture* renderTexture)
{
	float scaleX = currentHealth / maxHealth; //get percentage of sprint timer
	healthRect_.setPosition(renderTexture->mapPixelToCoords(sf::Vector2i(10, 80)));
	healthRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Player::updateMovement(const sf::Time& delta)
{
	//sf::Vector2f movement(0, 0);
	movement_ = sf::Vector2f(0, 0);
	sf::Vector2f direction(0, 0);
	collider_.top = getPosition().y - collider_.height / 2.f;
	collider_.left = getPosition().x - collider_.width / 2.f;
	if (canPunch)
	{
		punchCol_.top = collider_.top;
		punchCol_.left = collider_.left;
	}
	if (animationState == 1)
		playAnimation();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))	//press shift and sprint
	{
		sprint();
	}
	else //otherwise walk
	{
		walk();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{ //if a directional key is pressed
		if (animationState == 0)
			playAnimation();
		getDirection(direction);

		//create a vector that uses the two colliders and the direction to work out collisions
		sf::Vector2f a(direction.x * (delta.asSeconds() * moveSpeed), direction.y * (delta.asSeconds() * moveSpeed));

		movement_ = (p_tileMap_->getCollisionVector(collider_, a, getID()));
		normalizeMovement(movement_);
		move(movement_);	//move the player
	}
	else if (animationState == 0)
	{
		stopAnimation();
	}

}

void Player::updateRotation(const sf::Vector2f rotVec)
{
	rotationVector_ = rotVec;
	const float rotation((degrees(atan2(rotVec.y, rotVec.x))));
	setRotation(rotation);
}


void Player::punch()
{
	sf::Vector2f normalized;
	if (canPunch)
	{
		canPunch = false;
		if (rotationVector_.x != 0 && rotationVector_.y != 0)
		{
			normalized = normalize(rotationVector_);
		}

		normalized.x *= punchRange;
		normalized.y *= punchRange;

		punchCol_.left += normalized.x;
		punchCol_.top += normalized.y;
	}
	else if (!canPunch && punchClockStarted)
	{
		punchCol_.left = collider_.left + normalized.x;
		punchCol_.top = collider_.top + normalized.y;
	}
}

bool Player::invincibility()
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

void Player::punchTimer()
{
	if (canPunch == false)
	{
		if (punchClockStarted == false)
		{
			punchClockStarted = true;
			punchClock_.restart();
		}
		punchTimer_ = punchClock_.getElapsedTime();
		if (punchTimer_.asSeconds() > punchTime / 2)
		{
			punchCol_.left = collider_.left;
			punchCol_.top = collider_.top;
		}
		if (punchTimer_.asSeconds() > punchTime)
		{
			sf::Time frameTime = sf::milliseconds(150);
			setFrameTime(frameTime);
			setAnimationState(0);
			setAnimation(playerWalk_);
			canPunch = true;
			punchClockStarted = false;
		}
	}
}

void Player::pickupHealth(int amount)
{
	if (maxHealth - currentHealth < amount)
		currentHealth = maxHealth;
	else
		currentHealth += amount;
}

void Player::resetHealth()
{
	currentHealth = maxHealth;
}

void Player::getDirection(sf::Vector2f& direction)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{//if up key is pressed change direction y vector to -1 and move the y collider above the player
		direction.y = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{//if down key is pressed change direction y vector to 1 and move the y collider below the player
		direction.y = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{//if left key is pressed change direction x vector to -1 and move the x collider left of the player
		direction.x = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{//if left key is pressed change direction x vector to 1 and move the x collider right of the player
		direction.x = 1;
	}
}

void Player::normalizeMovement(sf::Vector2f& movement)
{
	if (movement.x != 0 && movement.y != 0) //if the movement vector is not (0,0)
	{
		sf::Vector2f normalized(normalize(movement));
		movement.x *= fabs(normalized.x);
		movement.y *= fabs(normalized.y);
	}
}

