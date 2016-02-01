#ifndef OBJECTS_H
#define OBJECTS_H
#include <SFML\Graphics.hpp>

class Objects
{
public:
	Objects();
	sf::FloatRect col_;
	sf::RectangleShape colShape_;
	int pickup();

private:
	int funcId;
};
#endif
