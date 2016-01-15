#ifndef LIGHTS
#define LIGHTS
#include <SFML\Graphics.hpp> 

class Light
{
public: 
	Light(int);
	void setTexture(const sf::Texture&);
private:
	const int type_;
	sf::RectangleShape shape_;
};

#endif