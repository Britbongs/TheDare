#include "Objects.h"
#include <iostream>
Objects::Objects() : funcId(0)
{
	col_.left = 128;
	col_.top = 1920;
	col_.width = 64;
	col_.height = 64;
	
	colShape_.setPosition(col_.left, col_.top);
	colShape_.setFillColor(sf::Color::Red);
	colShape_.setScale(64.f, 64.f);
	colShape_.setSize(sf::Vector2f(1, 1));
}

int Objects::pickup()
{
	//move the object so it can only be picked up once.
	col_.left = 0;
	col_.top = 0;
	colShape_.setPosition(col_.left, col_.top);
	return funcId; //return what function the object should have when picked up eg. increase ammo, health, add weapon etc.
}