#include "EnemyManager.h"

EnemyManager *EnemyManager::eManager_(nullptr);

EnemyManager::EnemyManager(sf::RenderTexture* render, Player* player, Bullet* bullets, TiledMap* map)
	: renderTexture_(render), bullets_(bullets), player_(player), map_(map)

{
	assert(eManager_ == nullptr); //Prevents us making 2 instances of the enemy manager
	eManager_ = this;
}

EnemyManager::~EnemyManager()
{
	//make emanager a null pointer
	eManager_ = nullptr;
	//free up any resources it used
	for (int i(0); i < static_cast<int> (enemies_.size()); ++i)
	{
		delete enemies_[i];
	}
}

EnemyManager* EnemyManager::Get()
{
	assert(eManager_ != nullptr);
	return eManager_;
}

bool EnemyManager::init()
{
	if (!txtSpriteSheet_.loadFromFile(gconsts::Assets::ENEMY_SPRITESHEET_TEXTURE))
		return(false);

	for (int i(0); i < gconsts::Gameplay::MAXENEMIES; ++i)
	{
		enemies_.push_back(new Enemy());

		enemies_[i]->setMap(map_);
		enemies_[i]->setScale(64.f, 64.f);
		//enemies_[i]->setAlive(false);
		enemies_[i]->setID(i + 1);
		enemies_[i]->setTexture(&txtSpriteSheet_);
		if (!enemies_[i]->init())
			return(false);
	}
	return(true);
}

void EnemyManager::update(const sf::Time & delta)
{

	//for (Enemy* e : enemies_)
	for (int i(0); i < enemies_.size();  ++i)
	{
		Enemy* e = enemies_[i];
		assert(e != nullptr);//Sanity checks

		if (e->getAlive())
		{
			const sf::Vector2f playerCentrePos(addVector(player_->getPosition(),
				sf::Vector2f(player_->getGlobalBounds().width / 2.f, player_->getGlobalBounds().height / 2.f)));

			//if (e->isChasing())
			//{
			handleRotation(e, playerCentrePos);
			e->update(delta, playerCentrePos);
			//}
			handleBulletCollision(e);
			handleCombat(e);
		}
	}
}

void EnemyManager::draw() const
{
	for (Enemy* e : enemies_)
	{//For each enemy in the enemies vector
		assert(e != nullptr); //sanity check  
		if (e->getAlive())
		{
			e->setOrigin(0.5f, 0.5f);
			//renderTexture_->draw(e->getChersBox());
			renderTexture_->draw(*e);
			renderTexture_->draw(e->getHealthRect());
			//renderTexture_->draw(e->colliderShape_);
			e->setOrigin(0.f, 0.f);
		}
	}
}

Enemy * EnemyManager::getEnemy(int index) const
{
	assert(index >= 0 && index < static_cast<int> (enemies_.size()));
	return (enemies_[index]);
}

void EnemyManager::reset()
{
	for (Enemy* e : enemies_)
	{
		e->setAlive(false);
		e->setPosition(-100.f, -100.f);
		e->setChasing(false);
		e->resetHealth();


	}
}

void EnemyManager::handleRotation(Enemy* e, const sf::Vector2f& playerCentrePos)
{
	assert(e != nullptr);
	sf::Vector2f rot(e->getPosition());
	rot = addVector(rot, sf::Vector2f((e->getGlobalBounds().width / 2.f), (e->getGlobalBounds().height / 2.f)));
	rot = subtractVector(playerCentrePos, rot);
	float enemyRotation = degrees(atan2(rot.y, rot.x));
	e->setRotation(enemyRotation);
}

void EnemyManager::handleBulletCollision(Enemy* enemy)
{
	for (int i(0); i < gconsts::Gameplay::MAXBULLETS; ++i)
	{
		assert(bullets_ != nullptr); //Sanity checks
		if (bullets_[i].getAlive())
		{
			if (bullets_[i].getCollider().intersects(enemy->getCollider()))
			{
				bullets_[i].setAlive(false);
				enemy->takeDamage(bullets_[i].getDamage());
				if (enemy->getCurrentHealth() <= 0)
				{
					enemy->kill();
				}
			}
		}
	}
}

void EnemyManager::handleCombat(Enemy* enemy)
{
	assert(enemy != nullptr);

	const sf::FloatRect tempPlayerCol(player_->getCollider().left - 2, player_->getCollider().top - 2, player_->getCollider().width + 4, player_->getCollider().height + 4);


	if (enemy->getChaseBox().intersects(player_->getCollider()))
	{
		enemy->setChasing(true);
	}

	if (player_->getAlive())
	{
		if (enemy->getCollider().intersects(tempPlayerCol) && player_->getCanTakeDamage())
		{
			player_->setCanTakeDamage(false);
			player_->takeDamage(enemy->getDamage());

			if (player_->getCurrentHealth() <= 0)
			{
				player_->setAlive(false);
			}
		}

		if (!player_->getCanTakeDamage())
		{
			if (player_->invincibility())
			{
				player_->setCanTakeDamage(true);
			}
			if (!player_->getCanPunch())
			{
				if (player_->getPunchCollider().intersects(enemy->getCollider())
					&& enemy->getCanTakeDamage())
				{
					if (enemy->getCanTakeDamage())
						enemy->takeDamage(player_->getPunchDamage());
					enemy->setCanTakeDamage(false);
					if (enemy->getCurrentHealth() <= 0)
					{
						enemy->kill();
					}
				}
			}
		}
		if (!enemy->getCanTakeDamage())
		{
			if (enemy->invincibility())
			{
				enemy->setCanTakeDamage(true);
			}
		}

	}

}