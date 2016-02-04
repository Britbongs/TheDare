#ifndef UTILS_H
#define UTILS_H

#define PI 3.14159
#define DIRS_SIZE 4

#include <cstdlib>

#include <SFML\Graphics.hpp>
#include <vector>

#include "..\Map\TiledMap.h"

using namespace std;

struct SATProjection
{
	SATProjection(double min, double max)
		: min(min), max(max)
	{

	}
	double min;
	double max;
};

struct Node
{
	Node() { }
	Node(sf::Vector2i pos = sf::Vector2i(0, 0), int f = 0, int g = 0, int h = 0) :
		pos(pos), f(f), g(g), h(h)
	{
	}

	sf::Vector2i pos;
	int f;
	int g;
	int h;
	sf::Vector2i parent;
};

sf::Vector2i dirs[DIRS_SIZE] =
{
	sf::Vector2i(-1, 0),
	sf::Vector2i(1, 0),
	sf::Vector2i(0, -1),
	sf::Vector2i(0, 1),
};

bool overlaps(SATProjection proj1, SATProjection proj2);

//bool collides(Entity* const entityOne, Entity* const entityTwo); 

/*SATProjection getProjection(sf::Vector2f normal, Entity* const entity);*/ 

float random(float a, float b);

float radians(float a); 

float degrees(float a);

float lerp(float v1, float v2, float mod);

vector<sf::Vector2i> aStarPath(sf::Vector2i start, sf::Vector2i end, const TiledMap& map);

#endif