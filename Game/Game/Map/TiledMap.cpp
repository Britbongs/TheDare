#include "TiledMap.h"
#include "..\Entities\Player.h"
#include "..\Entities\Enemy.h"

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

		for (int i(0); i < blocked_.size(); ++i)//Initialise the 2nd dimension with the right size
			blocked_[i].resize(mapBounds_.x);

		for (int i(0); i < firstGID_.size(); ++i)
		{
			firstGID_[i] = currentTMXMap_->getTileSet()[i]->firstgid_;
			tileCount_[i] = currentTMXMap_->getTileSet()[i]->tileCount_;
		}

		tempText.loadFromFile("res//tiles//tilesheet.png");

		initVertexArrays();
		setColTiles();

		for (int i(0); i < 8; ++i)
		{
			collisionArea_[i].collider.width = getTileWidth();
			collisionArea_[i].collider.height = getTileHeight();
		}

	}
	else
		return(false);

	return(true);
}

bool TiledMap::initVertexArrays()
{//Will initialise the vertex array with tiles to be rendered
	renderLayer_.resize(currentTMXMap_->getLayerCount());

	for (int i(0); i < renderLayer_.size(); ++i)
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

	for (int y(0); y < mapBounds_.y; ++y)
	{
		for (int x(0); x < mapBounds_.x; ++x)
		{
			if (currentTMXMap_->getLayer()[index]->data[y][x] != 0)
			{
				sf::Vertex* tile = &renderLayer_[index][((x + (y * mapBounds_.x)) * 4)];

				tile[0].position = sf::Vector2f(x * tileSize_.x, y * tileSize_.y);
				tile[1].position = sf::Vector2f((x + 1) * tileSize_.x, y * tileSize_.y);
				tile[2].position = sf::Vector2f((x + 1) * tileSize_.x, (y + 1) * tileSize_.y);
				tile[3].position = sf::Vector2f(x * tileSize_.x, (y + 1) * tileSize_.y);


				int tilesetID = getTilesetID(currentTMXMap_->getLayer()[index]->data[y][x]);

				if (tilesetID == -1) //Means no tilesets matched with the tile read in
					return(false);


				int tileID = currentTMXMap_->getLayer()[index]->data[y][x] - currentTMXMap_->getTileSet()[tilesetID]->firstgid_;

				int lx = tileID % (tempText.getSize().x / tileSize_.x);
				int ly = tileID / (tempText.getSize().x / tileSize_.y);

				tile[0].texCoords = sf::Vector2f(lx * tileSize_.x, ly * tileSize_.y);
				tile[1].texCoords = sf::Vector2f((lx + 1) * tileSize_.x, ly * tileSize_.y);
				tile[2].texCoords = sf::Vector2f((lx + 1) * tileSize_.x, (ly + 1) * tileSize_.y);
				tile[3].texCoords = sf::Vector2f(lx * tileSize_.x, (ly + 1) * tileSize_.y);
			}
		}

	}
	return (true);
}

void TiledMap::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
{
	renderStates.transform *= getTransform();
	renderStates.texture = &tempText;


	for (int i(0); i < renderLayer_.size(); ++i)
	{
		//Render each map layer
		renderTarget.draw(renderLayer_[i], renderStates);
	}
}

int TiledMap::getTilesetID(int tileID) const
{
	for (int i(0); i < firstGID_.size(); ++i)
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

void TiledMap::setPointers(Player* pplayer, const vector<Enemy>& penemies)
{
	p_player_ = pplayer;
	for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
	{
		p_enemies_[i] = &penemies[i];
	}
}

void TiledMap::setColTiles()
{

	float width = getTileWidth();
	float height = getTileHeight();
	int tilesetID(0);
	int tileID(-1);

	for (int y(0); y < currentTMXMap_->getLayer()[0]->data.size(); ++y)
	{
		for (int x(0); x < currentTMXMap_->getLayer()[0]->data[0].size(); x++)
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
	std::cout << blocked_.size() << " - " << blocked_[0].size() << std::endl;

}

sf::Vector2f TiledMap::getCollisionVector(sf::FloatRect collider, const sf::Vector2f& moveVector, const int id)
{
	assert(p_player_);
	sf::Vector2f moveBy(moveVector);

	//Get the grid location of the player
	sf::Vector2i gridLoc(collider.left / getTileWidth(), collider.top / getTileWidth());
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
	collisionArea_[1].collider.top = (gridLoc.y - 1) * getTileHeight();
	collisionArea_[1].collider.left = (gridLoc.x) * getTileWidth();

	collisionArea_[2].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x + 1]; //right-up tile
	collisionArea_[2].collider.top = (gridLoc.y - 1) * getTileHeight();
	collisionArea_[2].collider.left = (gridLoc.x + 1) * getTileWidth();

	collisionArea_[3].blockedValue = blocked_[gridLoc.y][gridLoc.x + 1]; //right tile
	collisionArea_[3].collider.top = (gridLoc.y) * getTileHeight();
	collisionArea_[3].collider.left = (gridLoc.x + 1) * getTileWidth();

	collisionArea_[4].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x + 1]; //right-down tile
	collisionArea_[4].collider.top = (gridLoc.y + 1) * getTileHeight();
	collisionArea_[4].collider.left = (gridLoc.x + 1) * getTileWidth();


	collisionArea_[5].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x]; //down tile
	collisionArea_[5].collider.top = (gridLoc.y + 1) * getTileHeight();
	collisionArea_[5].collider.left = (gridLoc.x) * getTileWidth();

	collisionArea_[6].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x - 1]; //down-left tile
	collisionArea_[6].collider.top = (gridLoc.y + 1) * getTileHeight();
	collisionArea_[6].collider.left = (gridLoc.x - 1) * getTileWidth();

	collisionArea_[7].blockedValue = blocked_[gridLoc.y][gridLoc.x - 1]; //left tile
	collisionArea_[7].collider.top = (gridLoc.y) * getTileHeight();
	collisionArea_[7].collider.left = (gridLoc.x - 1) * getTileWidth();

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
	if (id != p_player_->getID())
	{
		if (p_player_->getCollider().intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
		{
			moveBy.x *= -10.f;
		}
		if (p_player_->getCollider().intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
		{
			moveBy.y *= -10.f;
		}
		for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
		{

			if (i + 1 != id)
			{
				if (p_enemies_[i]->getCollider().intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
				{
					moveBy.x *= -3.f;
				}
				if (p_enemies_[i]->getCollider().intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
				{
					moveBy.y *= -3.f;
				}
			}
		}
	}
	if (id == 0)
	{
		for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
		{
			if (p_enemies_[i]->getCollider().intersects(sf::FloatRect(collider.left + moveVector.x, collider.top, collider.width, collider.height)))
			{
				moveBy.x = 0.f;
			}
			if (p_enemies_[i]->getCollider().intersects(sf::FloatRect(collider.left, collider.top + moveVector.y, collider.width, collider.height)))
			{
				moveBy.y = 0.f;
			}
		}
	}


	return(moveBy);
}

bool TiledMap::isTileBlocked(sf::Vector2i pos) const
{
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
	sf::Vector2i gridLoc(collider.left / getTileWidth(), collider.top / getTileWidth());

	collisionArea_[0].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x - 1]; //left-up tile
	collisionArea_[0].collider.top = (gridLoc.y - 1) * getTileHeight();
	collisionArea_[0].collider.left = (gridLoc.x - 1) * getTileWidth();

	collisionArea_[1].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x]; //up tile
	collisionArea_[1].collider.top = (gridLoc.y - 1) * getTileHeight();
	collisionArea_[1].collider.left = (gridLoc.x) * getTileWidth();

	collisionArea_[2].blockedValue = blocked_[gridLoc.y - 1][gridLoc.x + 1]; //right-up tile
	collisionArea_[2].collider.top = (gridLoc.y - 1) * getTileHeight();
	collisionArea_[2].collider.left = (gridLoc.x + 1) * getTileWidth();

	collisionArea_[3].blockedValue = blocked_[gridLoc.y][gridLoc.x + 1]; //right tile
	collisionArea_[3].collider.top = (gridLoc.y) * getTileHeight();
	collisionArea_[3].collider.left = (gridLoc.x + 1) * getTileWidth();

	collisionArea_[4].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x + 1]; //right-down tile
	collisionArea_[4].collider.top = (gridLoc.y + 1) * getTileHeight();
	collisionArea_[4].collider.left = (gridLoc.x + 1) * getTileWidth();


	collisionArea_[5].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x]; //down tile
	collisionArea_[5].collider.top = (gridLoc.y + 1) * getTileHeight();
	collisionArea_[5].collider.left = (gridLoc.x) * getTileWidth();

	collisionArea_[6].blockedValue = blocked_[gridLoc.y + 1][gridLoc.x - 1]; //down-left tile
	collisionArea_[6].collider.top = (gridLoc.y + 1) * getTileHeight();
	collisionArea_[6].collider.left = (gridLoc.x - 1) * getTileWidth();

	collisionArea_[7].blockedValue = blocked_[gridLoc.y][gridLoc.x - 1]; //left tile
	collisionArea_[7].collider.top = (gridLoc.y) * getTileHeight();
	collisionArea_[7].collider.left = (gridLoc.x - 1) * getTileWidth();

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