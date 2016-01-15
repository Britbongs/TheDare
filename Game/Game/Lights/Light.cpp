#include "Light.h"

Light::Light(int type, int orientation, const sf::Texture& texture) : type_(type)
{
	shape_.setTexture(&texture);
	shape_.setSize(sf::Vector2f(static_cast<float> (texture.getSize().x), static_cast<float> (texture.getSize().y)));
	switch (type)
	{
	case gconsts::Gameplay::POINT_LIGHT:
		orientation = -1;
		break;
	case gconsts::Gameplay::WALL_LIGHT:
		setupOrientation();
		break;
	}
}

void Light::render(sf::RenderTexture& target) const
{
	target.draw(shape_);
}

void Light::setScale(sf::Vector2f scale)
{
	shape_.setScale(scale);
}

void Light::setPosition(sf::Vector2f position)
{
	shape_.setPosition(position);
}

void Light::setupOrientation()
{
	/*
	Orientation list->
	0 = up
	1 = right
	2 = down
	3 = left
	*/
	if (orientation_ > 1)
	{
		switch (orientation_)
		{
		case 1:
			shape_.setRotation(90);//facing right
			break;
		case 2:
			shape_.setRotation(180); //facing down
			break;
		case 3:
			shape_.rotate(270); //facing left
			break;
		}
	}
}