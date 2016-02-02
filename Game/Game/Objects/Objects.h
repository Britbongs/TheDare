#ifndef OBJECTS_H
#define OBJECTS_H
#include <SFML\Graphics.hpp>

class Objects
	: public sf::Drawable, public sf::Transformable
{
public:
	Objects();
	Objects(int, int, int);//param : x/y pos , function ID (what pickup should do)
	bool initSpritesheet();

	int getFuncID() const { return funcID; }
	sf::FloatRect getCollider() const { return col_; }

	void pickup();
	void switchSprite(const int);
private:
	sf::FloatRect col_;
	sf::Texture propTexture_;
	sf::VertexArray vertices_;

	int funcID;
private:
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const;
};
#endif
