#include "Objects.h"

Objects::Objects()
{
	col_.left = 0;
	col_.top = 0;
	col_.width = 64;
	col_.height = 64;
}

Objects::Objects(float x, float y, int id, std::string text)
{
	col_.left = x;
	col_.top = y;
	col_.width = 64;
	col_.height = 64;

	funcID = id;

	text_ = text;
}





bool Objects::initSpritesheet()
{
	if (!propTexture_.loadFromFile("res//Tiles//propsheet.png"))
		return (false);

	vertices_.setPrimitiveType(sf::Quads);
	vertices_.resize(4);

	int tu = funcID % (256 / 128);
	int tv = funcID / (256 / 128);
	
	vertices_[0].position = sf::Vector2f(col_.left, col_.top);
	vertices_[1].position = sf::Vector2f(col_.left + 64, col_.top);
	vertices_[2].position = sf::Vector2f(col_.left + 64, col_.top + 64);
	vertices_[3].position = sf::Vector2f(col_.left, col_.top + 64);

	vertices_[0].texCoords = sf::Vector2f(static_cast<float>(tu * 64), static_cast<float>(tv * 64));
	vertices_[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * 64), static_cast<float>(tv * 64));
	vertices_[2].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * 64), static_cast<float>((tv + 1) * 64));
	vertices_[3].texCoords = sf::Vector2f(static_cast<float>(tu * 64), static_cast<float>((tv + 1) * 64));

	return true;
}

void Objects::pickup()
{
	//move the object so it can only be picked up once.
	col_.left = 0;
	col_.top = 0;

	vertices_[0].position = sf::Vector2f(col_.left, col_.top);
	vertices_[1].position = sf::Vector2f(col_.left + 64, col_.top);
	vertices_[2].position = sf::Vector2f(col_.left + 64, col_.top + 64);
	vertices_[3].position = sf::Vector2f(col_.left, col_.top + 64);

}

void Objects::switchSprite(const int a)
{
	int tu = a % (256 / 128);
	int tv = a / (256 / 128);

	vertices_[0].texCoords = sf::Vector2f(static_cast<float>(tu * 64), static_cast<float>(tv * 64));
	vertices_[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * 64), static_cast<float>(tv * 64));
	vertices_[2].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * 64), static_cast<float>((tv + 1) * 64));
	vertices_[3].texCoords = sf::Vector2f(static_cast<float>(tu * 64), static_cast<float>((tv + 1) * 64));
}

void Objects::draw(sf::RenderTarget& renderTarget_, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &propTexture_;
	renderTarget_.draw(vertices_, states);
}


