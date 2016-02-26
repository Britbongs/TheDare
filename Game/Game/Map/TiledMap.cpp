#include "TiledMap.h"
#include "..\Entities\Player.h"
#include "..\Entities\Enemy.h"
#include "..\Entities\EnemyManager.h"

TiledMap::TiledMap()
	: currentTMXMap_(nullptr), p_enemies_(gconsts::Gameplay::MAXENEMIES)
{

}
TiledMap::TiledMap(Map* tmxMap)
	: currentTMXMap_(tmxMap)
{
}

TiledMap::~TiledMap()
{
}

bool TiledMap::initaliseMap()
{
	if (currentTMXMap_)
	{
		mapBounds_.x = static_cast<unsigned> (currentTMXMap_->getWidth());
		mapBounds_.y = static_cast<unsigned> (currentTMXMap_->getHeight());
		tileSize_.x = static_cast<unsigned> (currentTMXMap_->getTileWidth());
		tileSize_.y = static_cast<unsigned> (currentTMXMap_->getTileHeight());

		firstGID_.resize(currentTMXMap_->getTileSet().size());
		tileCount_.resize(currentTMXMap_->getTileSet().size());

		blocked_.resize(mapBounds_.y); //resize the first dimension of the blocked map vector to the map bounds 

		for (int i(0); i < static_cast<int>(blocked_.size()); ++i)//Initialise the 2nd dimension with the right size
			blocked_[i].resize(mapBounds_.x);

		for (int i(0); i < static_cast<int>(firstGID_.size()); ++i)
		{
			firstGID_[i] = currentTMXMap_->getTileSet()[i]->firstgid_;
			tileCount_[i] = currentTMXMap_->getTileSet()[i]->tileCount_;
		}

		tempText.loadFromFile("res//tiles//tilesheet.png");

		initVertexArrays();
		setColTiles();

		for (int i(0); i < 8; ++i)
		{
			collisionArea_[i].collider.width = static_cast<float>(getTileWidth());
			collisionArea_[i].collider.height = static_cast<float>(getTileHeight());
		}

	}
	else
		return(false);

	return(true);
}

bool TiledMap::initVertexArrays()
{//Will initialise the vertex array with tiles to be rendered
	renderLayer_.resize(currentTMXMap_->getLayerCount());

	for (int i(0); i < static_cast<int>(renderLayer_.size()); ++i)
	{
		renderLayer_[i].setPrimitiveType(sf::Quads);
		renderLayer_[i].resize(mapBounds_.x * mapBounds_.y * 4);

		if (!initVertArray(i))
			return(false);
	}

	return(true);
}

bool TiledMap::initVertArray(int index)
{

	for (int y(0); y < static_cast<int>(mapBounds_.y); ++y)
	{
		for (int x(0); x < static_cast<int>(mapBounds_.x); ++x)
		{
			if (currentTMXMap_->getLayer()[index]->data[y][x] != 0)
			{
				sf::Vertex* tile = &renderLayer_[index][((x + (y * mapBounds_.x)) * 4)];

				tile[0].position = sf::Vector2f(static_cast<float>(x * tileSize_.x), static_cast<float>(y * tileSize_.y));
				tile[1].position = sf::Vector2f(static_cast<float>((x + 1) * tileSize_.x), static_cast<float>(y * tileSize_.y));
				tile[2].position = sf::Vector2f(static_cast<float>((x + 1) * tileSize_.x), static_cast<float>((y + 1) * tileSize_.y));
				tile[3].position = sf::Vector2f(static_cast<float>(x * tileSize_.x), static_cast<float>((y + 1) * tileSize_.y));


				int tilesetID = getTilesetID(currentTMXMap_->getLayer()[index]->data[y][x]);

				if (tilesetID == -1) //Means no tilesets matched with the tile read in
					return(false);


				int tileID = currentTMXMap_->getLayer()[index]->data[y][x] - currentTMXMap_->getTileSet()[tilesetID]->firstgid_;

				int lx = tileID % (tempText.getSize().x / tileSize_.x);
				int ly = tileID / (tempText.getSize().x / tileSize_.y);

				tile[0].texCoords = sf::Vector2f(static_cast<float>(lx * tileSize_.x), static_cast<float>(ly * tileSize_.y));
				tile[1].texCoords = sf::Vector2f(static_cast<float>((lx + 1) * tileSize_.x), static_cast<float>(ly * tileSize_.y));
				tile[2].texCoords = sf::Vector2f(static_cast<float>((lx + 1) * tileSize_.x), static_cast<float>((ly + 1) * tileSize_.y));
				tile[3].texCoords = sf::Vector2f(static_cast<float>(lx * tileSize_.x), static_cast<float>((ly + 1) * tileSize_.y));
			}
		}

	}
	return (true);
}

void TiledMap::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
{
	renderStates.transform *= getTransform();
	renderStates.texture = &tempText;


	for (int i(0); i < static_cast<int>(renderLayer_.size()); ++i)
	{
		//Render each map layer
		renderTarget.draw(renderLayer_[i], renderStates);
	}
}

int TiledMap::getTilesetID(int tileID) const
{
	for (int i(0); i < static_cast<int>(firstGID_.size()); ++i)
	{
		if (tileID >= firstGID_[i] && tileID < (firstGID_[i] + tileCount_[i]))
			return(i);
	}
	return(-1);
}

void TiledMap::setTMXFile(Map* m)
{
	assert(m != nullptr);
	currentTMXMap_ = m;
	blocked_.clear();
}

void TiledMap::setPointers(Player* pplayer, vector<Enemy>& penemies)
{
	p_player_ = pplayer;
	for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
	{
		p_enemies_[i] = &penemies[i];
	}
}

void TiledMap::setColTiles()
{

	int width = getTileWidth();
	int height = getTileHeight();
	int tilesetID(0);
	int tileID(-1);

	for (int y(0); y < static_cast<int>(currentTMXMap_->getLayer()[0]->data.size()); ++y)
	{
		for (int x(0); x < static_cast<int>(currentTMXMap_->getLayer()[0]->data[0].size()); x++)
		{
			tilesetID = getTilesetID(currentTMXMap_->getLayer()[0]->data[y][x]);
			if (tilesetID > -1)
			{
				tileID = currentTMXMap_->getLayer()[0]->data[y][x] - currentTMXMap_->getTileSet()[tilesetID]->firstgid_; // +firstGID_[tilesetID];
				if (currentTMXMap_->getTileSet()[tilesetID]->getTilePropertyName(tileID) == "blocked" && currentTMXMap_->getTileSet()[tilesetID]->getTilePropertyValue(tileID) == "true")
				{
					blocked_[y][x] = 1;
				}
				else
				{
					blocked_[y][x] = 0;
				}
			}
			else
			{
				blocked_[y][x] = 0;
			}
		}

	}
	//std::cout << blocked_.size() << " - " << blocked_[0].size() << std::endl;

}

sf::Vector2f TiledMap::getCollisionVector(sf::FloatRect collider, const sf::Vector2f& moveVector, const int id)
{
	assert(p_player_);
	sf::Vector2f moveBy(moveVector);

	//Get the grid location of the player
	sf::Vector2i gridLoc(static_cast<int>(collider.left / getTileWidth()), static_cast<int>(collider.top / getTileWidth()));
	/*
	Collision Area order =
	left-up = (-1,-1)
	up = (0,-1)
	right-up = (1,-1)
	right = (1, 0)
	right-down = (1,1)
	down = (0,1)
	left-down = (-1,1)
	left = (-1,0)
	*/
	collisionArea_[0].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x - 1]; //left-up tile
	collisionArea_[0].collider.top = static_cast<float>((gridLoc.y - 1) * getTileHeight());
	collisionArea_[0].collider.left = static_cast<float>((gridLoc.x - 1) * getTileWidth());

	collisionArea_[1].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x]; //up tile
	collisionArea_[1].collider.top = static_cast<float>((gridLoc.y - 1) * getTileHeight());
	collisionArea_[1].collider.left = static_cast<float>((gridLoc.x) * getTileWidth());

	collisionArea_[2].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x + 1]; //right-up tile
	collisionArea_[2].collider.top = static_cast<float>((gridLoc.y - 1) * getTileHeight());
	collisionArea_[2].collider.left = static_cast<float>((gridLoc.x + 1) * getTileWidth());

	collisionArea_[3].blockedValue = blocked_[gridLoc.y][gridLoc.x + 1]; //right tile
	collisionArea_[3].collider.top = static_cast<float>((gridLoc.y) * getTileHeight());
	collisionArea_[3].collider.left = static_cast<float>((gridLoc.x + 1) * getTileWidth());

	collisionArea_[4].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x + 1]; //right-down tile
	collisionArea_[4].collider.top = static_cast<float>((gridLoc.y + 1) * getTileHeight());
	collisionArea_[4].collider.left = static_cast<float>((gridLoc.x + 1) * getTileWidth());


	collisionArea_[5].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x]; //down tile
	collisionArea_[5].collider.top = static_cast<float>((gridLoc.y + 1) * getTileHeight());
	collisionArea_[5].collider.left = static_cast<float>((gridLoc.x) * getTileWidth());

	collisionArea_[6].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x - 1]; //down-left tile
	collisionArea_[6].collider.top = static_cast<float>((gridLoc.y + 1) * getTileHeight());
	collisionArea_[6].collider.left = static_cast<float>((gridLoc.x - 1) * getTileWidth());

	collisionArea_[7].blockedValue = blocked_[gridLoc.y][gridLoc.x - 1]; //left tile
	collisionArea_[7].collider.top = static_cast<float>((gridLoc.y) * getTileHeight());
	collisionArea_[7].collider.left = static_cast<float>((gridLoc.x - 1) * getTileWidth());

	for (int i(0); i < 8; ++i)
	{
		if (collisionArea_[i].blockedValue == 1)
		{
			//if there is a horizontal collision
			if (collisionArea_[i].collider.intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
			{
				moveBy.x = 0.f;
			}
			//if there is a vertical collision
			if (collisionArea_[i].collider.intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
			{
				moveBy.y = 0.f;
			}
		}

	}

	EnemyManager* eManage(EnemyManager::Get());

	assert(eManage != nullptr);

	if (id != 0)
	{

		if (p_player_->getCollider().intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
		{
			moveBy.x = 0.f;
		}

		if (p_player_->getCollider().intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
		{
			moveBy.y = 0.f;
		}

		for (int i(0); i < eManage->getEnemyCount(); ++i)
		{
			Enemy* e(eManage->getEnemy(i));

			assert(e != nullptr); //sanity checks

			if (i + 1 != id)
			{
				if (e->getAlive())
				{
					if (e->getCollider().intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
					{
						e->collidedX_ = true;
						moveBy.x *= -1.f;
						if (e->getCollider().intersects(sf::FloatRect(collider.left + (moveVector.x + moveBy.x), collider.top, collider.width, collider.height)))
						{
							moveBy.x *= -1.f;
						}
					}
					else
					{
						e->collidedX_ = false;
					}
					if (e->getCollider().intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
					{
						e->collidedY_ = true;
						moveBy.y *= -1.f;
						if (e->getCollider().intersects(sf::FloatRect(collider.left, collider.top + (moveVector.y + moveBy.y), collider.width, collider.height)))
						{
							moveBy.y *= -1.f;
						}
					}

					else
					{
						e->collidedY_ = false;
					}
				}
			}
		}
	}
	else
	{
		for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
		{
			Enemy* e(eManage->getEnemy(i));
			if (e->getAlive())
			{
				assert(e != nullptr); //sanity checks

				if (e->getCollider().intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
				{
					moveBy.x = 0.f;
				}
				if (e->getCollider().intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
				{
					moveBy.y = 0.f;
				}
			}
		}
	}

	return(moveBy);
}

bool TiledMap::isTileBlocked(sf::Vector2i pos) const
{
	assert(pos.x >= 0 && pos.x < getWidthInTiles() && pos.y >= 0 && pos.y <= getHeightInTiles());
	return(blocked_[pos.y][pos.x] == 1);
}

bool TiledMap::isCollided(sf::FloatRect collider, const sf::Vector2f& moveVector)
{

	/*
	Collision Area order =
	left-up = (-1,-1)
	up = (0,-1)
	right-up = (1,-1)
	right = (1, 0)
	right-down = (1,1)
	down = (0,1)
	left-down = (-1,1)
	left = (-1,0)
	*/
	sf::Vector2i gridLoc(static_cast<int>(collider.left / getTileWidth()), static_cast<int>(collider.top / getTileWidth()));

	collisionArea_[0].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x - 1]; //left-up tile
	collisionArea_[0].collider.top = static_cast<float>((gridLoc.y - 1) * getTileHeight());
	collisionArea_[0].collider.left = static_cast<float>((gridLoc.x - 1) * getTileWidth());

	collisionArea_[1].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x]; //up tile
	collisionArea_[1].collider.top = static_cast<float>((gridLoc.y - 1) * getTileHeight());
	collisionArea_[1].collider.left = static_cast<float>((gridLoc.x) * getTileWidth());

	collisionArea_[2].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x + 1]; //right-up tile
	collisionArea_[2].collider.top = static_cast<float>((gridLoc.y - 1) * getTileHeight());
	collisionArea_[2].collider.left = static_cast<float>((gridLoc.x + 1) * getTileWidth());

	collisionArea_[3].blockedValue = blocked_[gridLoc.y][gridLoc.x + 1]; //right tile
	collisionArea_[3].collider.top = static_cast<float>((gridLoc.y) * getTileHeight());
	collisionArea_[3].collider.left = static_cast<float>((gridLoc.x + 1) * getTileWidth());

	collisionArea_[4].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x + 1]; //right-down tile
	collisionArea_[4].collider.top = static_cast<float>((gridLoc.y + 1) * getTileHeight());
	collisionArea_[4].collider.left = static_cast<float>((gridLoc.x + 1) * getTileWidth());


	collisionArea_[5].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x]; //down tile
	collisionArea_[5].collider.top = static_cast<float>((gridLoc.y + 1) * getTileHeight());
	collisionArea_[5].collider.left = static_cast<float>((gridLoc.x) * getTileWidth());

	collisionArea_[6].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x - 1]; //down-left tile
	collisionArea_[6].collider.top = static_cast<float>((gridLoc.y + 1) * getTileHeight());
	collisionArea_[6].collider.left = static_cast<float>((gridLoc.x - 1) * getTileWidth());

	collisionArea_[7].blockedValue = blocked_[gridLoc.y][gridLoc.x - 1]; //left tile
	collisionArea_[7].collider.top = static_cast<float>((gridLoc.y) * getTileHeight());
	collisionArea_[7].collider.left = static_cast<float>((gridLoc.x - 1) * getTileWidth());

	for (int i(0); i < 8; ++i)
	{
		if (collisionArea_[i].blockedValue == 1)
		{
			if (collisionArea_[i].collider.intersects(sf::FloatRect(collider.left + moveVector.x, collider.top + moveVector.y, collider.width, collider.height)))
				return(true);
		}
	}

	return false;

}

int TiledMap::getTileWidth() const
{
	return(currentTMXMap_->getTileWidth());
}
int TiledMap::getTileHeight() const
{
	return(currentTMXMap_->getTileHeight());
}