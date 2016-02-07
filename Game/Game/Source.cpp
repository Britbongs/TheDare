#include <vector>
#include <SFML\Graphics.hpp>


using namespace std;

int main(void)
{

	sf::RenderWindow window;
	window.create(sf::VideoMode(1024, 640), "Map Object Test", sf::Style::Close);
	sf::Clock clock;
	sf::Texture txt;
	if (!txt.loadFromFile("res//icon.png"))
		system("pause");
	sf::Image img(txt.copyToImage());

	const int SIZE(256);

	window.setIcon(SIZE, SIZE,img.getPixelsPtr() );

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

		window.clear(sf::Color::Black);
		window.display();
	}

	//delete[] a;
}