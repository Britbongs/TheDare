#include "Utils.h"

bool overlaps(SATProjection proj1, SATProjection proj2)
{//Checks if two values overlap for SAT collision detection
	return(proj1.max >= proj2.min && proj1.min < proj2.max);
}

float random(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;

}

float radians(float a)
{
	return((PI / 180.f) * a);
}

float degrees(float a)
{
	return((180.f / PI) * a);
}

float lerp(float v1, float v2, float mod)
{
	return(v1 + ((v2 - v1) * mod));
}

int getListIndex(const vector<Node>& list, sf::Vector2i pos)
{
	int count(0);
	bool found(false);
	while (count < list.size() && !found)
	{
		if (list[count].pos == pos)
			found = true;
		++count;
	}
	return(count - 1);
}

vector<sf::Vector2i> createVectorPath(vector<Node>& list)
{

	vector <sf::Vector2i> a;
	if (list.size() > 0)
	{

		const sf::Vector2i start = list.front().pos;
		const sf::Vector2i end = list.back().pos;

		Node node(list.back());


		while (node.pos != start)
		{
			a.push_back(node.pos);
			node = list[getListIndex(list, node.parent)];
		}
		a.push_back(start);

		std::reverse(a.begin(), a.end());
	}

	return(a);
}

vector<Node> getAdjacentTiles(sf::Vector2i location, const TiledMap& map)
{
	vector<Node> tiles;
	/*
	left, right, up, down
	*/
	for (int i(0); i < DIRS_SIZE; ++i)
	{
		int modX(location.x + dirs[i].x), modY(location.y + dirs[i].y);

		if (modX >= 0 && modX < map.getTileWidth() && modY >= 0 && modY < map.getTileHeight())
		{
			if (!map.isTileBlocked(sf::Vector2i(modX, modY)))
			{
				tiles.push_back(sf::Vector2i(modX, modY));
			}

		}
	}
	return(tiles);
}

bool isInList(const vector<Node>& list, Node node)
{
	int i(0);
	bool found(false);
	while (i < list.size() && !found)
	{
		if (list[i].pos.x == node.pos.x && list[i].pos.y == node.pos.y)
		{
			found = true;
		}
		++i;
	}
	return(found);
}

int getGScore(int parentScore)
{
	return(parentScore + 1);
}

int getHScore(sf::Vector2i start, sf::Vector2i end)
{
	return(abs((end.x - start.x) + abs(end.y - start.y)));
}

int getFScore(int hScore, int gScore)
{//Will compute the f score by F = G+H 
	return(hScore + gScore);
}

Node getLowestFScoreNode(const vector<Node>& list, int& idx)
{//Will search through a list and find the node with the lowest F value
	assert(list.size() > 0);
	int index(0);
	int value(list[0].f);

	for (int i(0); i < list.size(); ++i)
	{
		if (list[i].f < value)
		{
			value = list[i].f;
			index = i;
		}
	}
	idx = index;
	return(list[index]);
}
vector<sf::Vector2i> aStarPath(sf::Vector2i start, sf::Vector2i end, const TiledMap& map)
{
	//G = the G of the parent square + 1
	//H = Manhatten length estimate to target
	int getListIndex(const vector<Node>& list, sf::Vector2i pos);
	vector<sf::Vector2i> createVectorPath(vector<Node>& list); 
	vector<Node> getAdjacentTiles(sf::Vector2i location, const TiledMap& map);
	bool isInList(const vector<Node>& list, Node node); 
	int getGScore(int parentScore);
	int getHScore(sf::Vector2i start, sf::Vector2i end);
	int getFScore(int hScore, int gScore); 
	Node getLowestFScoreNode(const vector<Node>& list, int& idx); 

	vector<Node> closedList;
	vector<Node> openList;

	if (map.isTileBlocked(end))
		return(createVectorPath(closedList));

	openList.push_back(Node(start));
	vector<Node> adjacentTiles;

	do
	{
		int index(0);
		Node current(getLowestFScoreNode(openList, index));
		Node* current_p(nullptr);
		//openList.pop_back();

		closedList.push_back(current);

		current_p = &closedList.back();

		vector<Node>::iterator it = openList.begin() + index;

		openList.erase(it);

		if (isInList(closedList, end))
		{//If the end tile is in the closed list, then we've completed our algorithm
			return createVectorPath(closedList);
		}
		adjacentTiles.clear();
		adjacentTiles = getAdjacentTiles(current.pos, map);

		for (Node& n : adjacentTiles)
		{//Loop through all adjacent tiles to the current tile we're checking
			if (!isInList(closedList, n))
			{
				if (!isInList(openList, n))
				{//if the adjacent tile isn't in the list, calculate it's scores and add it to the open list
					n.g = getGScore(current.g);
					n.h = getHScore(n.pos, end);
					n.f = n.g + n.h;
					n.parent = current.pos;
					openList.push_back(n);
				}
				else
				{
					//If using the G score of the current tile + the adjacent tiles H
					//calculates a lower F score
					if ((current.g + n.h) < n.f)
					{//Update the parent, cause it means a better path
						(*current_p).f = (current.g + n.h) - 1; //Set the parent's F to a new value
					}
				}
			}
		}

	} while (!openList.empty());


	return(createVectorPath(closedList));
}