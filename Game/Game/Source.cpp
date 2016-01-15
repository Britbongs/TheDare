#include <vector>
#include <SFML\Graphics.hpp>
#include "Map\TiledMap.h"
#include "Utils\MTileMap.h"
#include "Entities\Player.h"
#include "Utils\Animation.hpp"

using namespace std;

int main(void)
{
	MTileMap tmx;
	TiledMap map;

	tmx.loadMap("res//lel.tmx");
	map.setTMXFile(&tmx);
	map.initaliseMap();

	sf::RenderWindow window;
	window.create(sf::VideoMode(1024, 640), "Map Object Test", sf::Style::Close);
	sf::Clock clock;
	sf::Texture txt;
	txt.loadFromFile("res//entities//spritesheet.png");

	while (window.isOpen())
	{
		sf::Time  delta(clock.restart());
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
				window.close();
			if (evnt.type == sf::Event::KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}
		
		window.clear(sf::Color::Cyan);
		window.display();
	}
}