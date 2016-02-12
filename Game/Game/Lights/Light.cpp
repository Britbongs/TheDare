#include "Light.h"

Light::Light(int type, int orientation, const sf::Texture& texture, const bool flicker, const bool offOnCollide, const float timer, const int r, const int g, const int b, const int a)
	: type_(type), orientation_(orientation), flicker_(flicker), offOnCollide_(offOnCollide), flickerTime_(timer), lit_(true), broke_(false)
{
	shape_.setTexture(&texture);
	shape_.setSize(sf::Vector2f(static_cast<float> (texture.getSize().x), static_cast<float> (texture.getSize().y)));

	sf::Color colour(r, g, b, a);
	shape_.setFillColor(colour);
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

void Light::flicker()
{
	if (flicker_)
	{

		time_ = timer_.getElapsedTime();
		if (time_.asSeconds() >= flickerTime_)
		{
			timer_.restart();
			lit_ = !lit_;
		}
	}
	else
	{
		timer_.restart();
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
	switch (type_)
	{
	case gconsts::Gameplay::POINT_LIGHT:
		position.x -= shape_.getSize().x / 2.f;
		position.y -= shape_.getSize().x / 2.f;
		break;
	case gconsts::Gameplay::WALL_LIGHT:
		//sf::Color g( 250, 243, 165, 255);
		//shape_.setFillColor(g);
		shape_.setOrigin(shape_.getLocalBounds().width / 2.f, shape_.getLocalBounds().height / 2.f);
		position.x += gconsts::Gameplay::TILESIZE;//tilesize
		position.y -= gconsts::Gameplay::TILESIZE;
		break;
	}
	shape_.setPosition(position);

}

void Light::setColour(const sf::Color colour)
{
	shape_.setFillColor(colour);
}

void Light::reset()
{
	broke_ = false;
	lit_ = true;
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