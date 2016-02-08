#include <iostream>	
#include <queue>
#include <tuple>
#include <vector>
#include <assert.h>

#include "Utils\MTileMap.h"
#include "Utils\Utils.h"
#include "Utils\VectorUtils.h"
#include "Map\TiledMap.h"

using namespace std;

const int TILESIZE(64);
//const int DIRS_SIZE(4);
//const int DIRS_SIZE(8);

/*
sf::Vector2i dirs[8] =
{
	sf::Vector2i(-1, 0),
	sf::Vector2i(1, 0),
	sf::Vector2i(0, -1),
	sf::Vector2i(0, 1),
	sf::Vector2i(-1, -1),
	sf::Vector2i(1, -1),
	sf::Vector2i(-1, 1),
	sf::Vector2i(1, 1),
};
*/
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

void highlightPath(sf::VertexArray& highlight, const vector<sf::Vector2i>& path)
{
	highlight.clear();
	//highlight.resize((path.size() - 2) * 5);
	highlight.setPrimitiveType(sf::LinesStrip);
	sf::Vertex vertices[5];
	for (int i(0); i < path.size(); ++i)
	{
		vertices[0].color = sf::Color::Blue;
		vertices[1].color = sf::Color::Blue;
		vertices[2].color = sf::Color::Blue;
		vertices[3].color = sf::Color::Blue;
		vertices[4].color = sf::Color::Blue;

		vertices[0].position = sf::Vector2f(path[i].x * TILESIZE, path[i].y * TILESIZE);
		vertices[1].position = sf::Vector2f((path[i].x + 1) * TILESIZE, path[i].y * TILESIZE);
		vertices[2].position = sf::Vector2f((path[i].x + 1) * TILESIZE, (path[i].y + 1)* TILESIZE);
		vertices[3].position = sf::Vector2f(path[i].x * TILESIZE, (path[i].y + 1) * TILESIZE);
		vertices[4].position = vertices[0].position;

		highlight.append(vertices[0]);
		highlight.append(vertices[1]);
		highlight.append(vertices[2]);
		highlight.append(vertices[3]);
		highlight.append(vertices[4]);
	}
}

bool lerpEnemyToPos(sf::RectangleShape& e, sf::Vector2f target, float& mod, float delta)
{
	if (mod < 1.f)
	{
		

		float xpos = lerp(e.getPosition().x, target.x, mod);
		float ypos = lerp(e.getPosition().y, target.y, mod);

		e.setPosition(xpos, ypos);
		mod += delta;
		return(true);
	}
	mod = 0.f;
	return(false);
}

int main(void)
{
	//vector<Vector2i> a = aStarPath(Vector2i(0, 0), Vector2i(2, 1));


	MTileMap tmx;
	tmx.loadMap("res//levels//level1.tmx");
	TiledMap tiledMap;
	vector<sf::Vector2i> path;

	tiledMap.setTMXFile(&tmx);
	tiledMap.initaliseMap();

	//vector<sf::Vector2i> a = aStarPath(sf::Vector2i(0, 0), sf::Vector2i(3, 4), tiledMap);
	sf::RenderWindow window;

	window.create(sf::VideoMode(1024, 640), "A* Test");
	window.setKeyRepeatEnabled(false);

	float SPEED = 150.f;
	sf::Clock c;

	sf::VertexArray startHighlight(sf::LinesStrip, 5);
	sf::VertexArray endHighlight(sf::LinesStrip, 5);
	sf::VertexArray pathHighlight(sf::LinesStrip);

	for (int i(0); i < startHighlight.getVertexCount(); ++i)
	{
		startHighlight[i].color = sf::Color::Green;
		endHighlight[i].color = sf::Color::Red;
	}

	sf::RectangleShape enemy(sf::Vector2f(64.f, 64.f));
	enemy.setFillColor(sf::Color::Cyan);

	sf::Vector2f target(-1.f, -1.f);
	int index = 1;
	float mod(0.f);

	while (window.isOpen())
	{

		sf::Time delta(c.restart());
		sf::Event evnt;

		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::MouseButtonPressed)
			{
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{

					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					sf::Vector2i grid(floor(mousePos.x / TILESIZE), floor(mousePos.y / TILESIZE));

					startHighlight[0].position = sf::Vector2f(grid.x * TILESIZE, grid.y * TILESIZE);
					startHighlight[1].position = sf::Vector2f((grid.x + 1) * TILESIZE, grid.y * TILESIZE);
					startHighlight[2].position = sf::Vector2f((grid.x + 1) * TILESIZE, (grid.y + 1) * TILESIZE);
					startHighlight[3].position = sf::Vector2f(grid.x * TILESIZE, (grid.y + 1) * TILESIZE);
					startHighlight[4].position = startHighlight[0].position;
				}

				if (evnt.mouseButton.button == sf::Mouse::Right)
				{
					index = 1;
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					sf::Vector2i grid(floor(mousePos.x / TILESIZE), floor(mousePos.y / TILESIZE));

					endHighlight[0].position = sf::Vector2f(grid.x * TILESIZE, grid.y * TILESIZE);
					endHighlight[1].position = sf::Vector2f((grid.x + 1) * TILESIZE, grid.y * TILESIZE);
					endHighlight[2].position = sf::Vector2f((grid.x + 1) * TILESIZE, (grid.y + 1) * TILESIZE);
					endHighlight[3].position = sf::Vector2f(grid.x * TILESIZE, (grid.y + 1) * TILESIZE);
					endHighlight[4].position = endHighlight[0].position;

					sf::Vector2i startGrid(static_cast<int> (startHighlight[0].position.x / TILESIZE), static_cast<int>(startHighlight[0].position.y / TILESIZE));
					sf::Vector2i endGrid(static_cast<int> (endHighlight[0].position.x / TILESIZE), static_cast<int> (endHighlight[0].position.y / TILESIZE));

					path = aStarPath(startGrid, endGrid, tiledMap);

					highlightPath(pathHighlight, path);
					enemy.setPosition(startHighlight[0].position);
					target = sf::Vector2f(path[index].x * TILESIZE, path[index].y * TILESIZE);
				}
			}
			if (evnt.type == sf::Event::Closed)
				window.close();

			if (evnt.type == sf::Event::KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Escape)
					window.close();
			}


		}
		if (target.x != -1 && target.y != -1)
		{
			if (!lerpEnemyToPos(enemy, target, mod, delta.asSeconds()))
			{
				++index; 
				if (index < path.size())
				{
					target = sf::Vector2f(path[index].x * TILESIZE, path[index].y * TILESIZE);
				}
				else

				{
					target.x = -1.f; 
					target.y = -1.f;
					index = 1;
				}
			}
		}
		cout << mod << endl;

		sf::View v(window.getView());

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			v.move(-SPEED * delta.asSeconds(), 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			v.move(SPEED * delta.asSeconds(), 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			v.move(0.f, -SPEED * delta.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			v.move(0.f, SPEED * delta.asSeconds());
		}
		window.setView(v);


		window.clear(sf::Color::Black);
		window.draw(tiledMap);
		window.draw(startHighlight);
		window.draw(endHighlight);
		window.draw(pathHighlight);
		window.draw(enemy);
		window.display();
	}

	system("pause");
	return(0);
}