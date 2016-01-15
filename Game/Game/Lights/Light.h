#ifndef LIGHTS
#define LIGHTS
#include <SFML\Graphics.hpp> 
#include "..\Constants\Constants.h"

class Light
{
public: 
	
	Light(int, int, const sf::Texture&);

	void setScale(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void render(sf::RenderTexture&) const;

private://Functions
	void setupOrientation(); //rotate the shape to the right orientation

private: //Data memebers
	const int type_;
	int orientation_;
	sf::RectangleShape shape_;

};
#endif