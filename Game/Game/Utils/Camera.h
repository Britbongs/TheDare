#ifndef CAMERA_H
#define CAMERA_H
#include <SFML\Graphics.hpp> 
#include <iostream>
#include <assert.h>
#include "..\Utils\VectorUtils.h"
#include "..\Constants\Constants.h"	


class Camera
{
public:
	Camera(sf::Vector2u, sf::RenderTexture*);
	~Camera();
	//Param: sf::Time delta time, sf::Vector2f player centre, Boolean is the player moving
	void update(const sf::Time&, const sf::Vector2f&, bool, const sf::Vector2f&); 
private:
	sf::RenderTexture* renderTexture_;
	const float MOVE_SPEED = 15.f; 
	const int TILESIZE;
	const float lag = 0.25;
	float mod_;
	bool lerpCam_;
};
#endif