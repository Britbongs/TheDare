#ifndef OBJECTS_H
#define OBJECTS_H
#include <SFML\Graphics.hpp>
#include <string>

class Objects
	: public sf::Drawable, public sf::Transformable
{
public:
	Objects();
	Objects(float, float, int, int);//param : x/y pos , function ID (what pickup should do)
	bool initSpritesheet();

	int getFuncID() const { return funcID; }
	int getTextureID() const { return textureID; }
	sf::FloatRect getCollider() const { return col_; }
	std::string getText() const { return text_; }

	void pickup();
	void switchSprite(const int);
private:
	sf::FloatRect col_;
	sf::Texture propTexture_;
	sf::VertexArray vertices_;
	std::string text_;
	int funcID;
	int textureID;
private:
	virtual void draw(sf::RenderTarget&,sf::RenderStates) const;
};
#endif
