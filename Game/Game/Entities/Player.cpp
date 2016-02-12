#include "Player.h"	
#include <iostream>
Player::Player()
	: moveSpeed(250), maxSprint(500), sprintTime(maxSprint), maxHealth(100), currentHealth(maxHealth), alive(true), invinClockStarted(false), canTakeDamage(true), invincTime(1.f),
	punchRange(40.f), canPunch(true), punchClockStarted(false), punchTime(0.3f), punchDamage(25.f), animationState(0), sprinting(false)
{
}
Player::~Player()
{
}
bool Player::init()
{
	if (!initSpritesheet())
		return(false);

	collider_.width = static_cast<float>(getGlobalBounds().width * 0.90);
	collider_.height = static_cast<float>(getGlobalBounds().height * 0.90);
	collider_.top = -64;
	collider_.left = -64;

	punchCol_.width = static_cast<float>(getGlobalBounds().width * 0.90);
	punchCol_.height = static_cast<float>(getGlobalBounds().height * 0.90);

	colShape_.setScale(punchCol_.width, punchCol_.height);
	colShape_.setSize(sf::Vector2f(1, 1));
	colShape_.setPosition(punchCol_.left, punchCol_.top);
	colShape_.setFillColor(sf::Color::Green);

	sprintRect_.setFillColor(sf::Color::Red); //init sprint rect with colour red
	sprintRect_.setSize(sf::Vector2f(120, 10)); //init sprint rect with width of player and size of 5
	sprintBox_.setSize(sf::Vector2f(150, 20));

	healthRect_.setFillColor(sf::Color::Green); //init sprint rect with colour red
	healthRect_.setSize(sf::Vector2f(120, 10)); //init sprint rect with width of player and size of 5
	healthBox_.setSize(sf::Vector2f(150, 20));


	aManage_ = AudioManager::Get();

	if (!initAudio())
		return(false);
	return(true);

}

bool Player::initSpritesheet()
{
	if (!spritesheet_.loadFromFile("res//entities//spritesheet.png"))
		return(false);
	if (!sprintTexture_.loadFromFile("res//illustrations//staminaBar.png"))
		return(false);
	if (!healthTexture_.loadFromFile("res//illustrations//healthBar.png"))
		return(false);

	sprintBox_.setTexture(&sprintTexture_);
	healthBox_.setTexture(&healthTexture_);

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

bool Player::initAudio()
{	
	footsteps_.setBuffer(aManage_->buffers_[aManage_->FOOTSTEP]);
	footsteps_.setLoop(1);
	footsteps_.setVolume(20);

	hurtSnd_.setBuffer(aManage_->buffers_[aManage_->PLAYER_HURT]);
	hurtSnd_.setLoop(0);
	hurtSnd_.setVolume(20);

	deathSnd_.setBuffer(aManage_->buffers_[aManage_->PLAYER_DEATH]);
	deathSnd_.setLoop(0);
	deathSnd_.setVolume(40);
	
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
		footsteps_.setPitch(2);
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
	footsteps_.setPitch(1.3f);
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
	sprintBox_.setPosition(renderTexture->mapPixelToCoords(sf::Vector2i(10, 130)));
	sprintRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
	sprintRect_.setPosition(sprintBox_.getPosition().x + 25, sprintBox_.getPosition().y + 5);

}

void Player::updateHealthBar(const sf::RenderTexture* renderTexture)
{
	float scaleX = currentHealth / maxHealth; //get percentage of sprint timer
	healthBox_.setPosition(renderTexture->mapPixelToCoords(sf::Vector2i(10, 80)));
	healthRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
	healthRect_.setPosition(healthBox_.getPosition().x + 25, healthBox_.getPosition().y + 5);
}

void Player::updateMovement(const sf::Time& delta)
{
	//sf::Vector2f movement(0, 0);
	movement_ = sf::Vector2f(0, 0);
	sf::Vector2f direction(0, 0);
	collider_.top = getPosition().y - collider_.height / 2.f;
	collider_.left = getPosition().x - collider_.width / 2.f;
	colShape_.setPosition(punchCol_.left, punchCol_.top);
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
		if (!stepSoundPlaying)
		{
			stepSoundPlaying = true;
			footsteps_.play();
		}
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
		stepSoundPlaying = false;
		footsteps_.stop();
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
	if (canPunch) //if player is allowed to punch
	{
		canPunch = false;
		if (rotationVector_.x != 0 && rotationVector_.y != 0) // if the rotation x and y dont equal zero
		{
			normalized = normalize(rotationVector_); //normalize the vector 
		}

		normalized.x *= punchRange; 
		normalized.y *= punchRange;

		punchCol_.left += normalized.x;
		punchCol_.top += normalized.y;
	}
	else if (!canPunch)
	{
		punchCol_.left = collider_.left + normalized.x;
		punchCol_.top = collider_.top + normalized.y;
	}
	std::cout << "Punching" << std::endl;
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

void Player::takeDamage(const float damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0)
	{
		deathSnd_.play();
	}
	else
	{
		hurtSnd_.play();
	}
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

