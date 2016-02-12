#include "Camera.h"

Camera::Camera(sf::Vector2u MAP_BOUNDS, sf::RenderTexture* renderTexture)
	: renderTexture_(renderTexture), mod_(0.f), lerpCam_(false), TILESIZE(gconsts::Gameplay::TILESIZE)
{
}

Camera::~Camera()
{

}

void Camera::update(const sf::Time& delta, const sf::Vector2f& position, bool isPlayerSprinting, const sf::Vector2f& playerMoveVec)
{
	sf::View currentView(renderTexture_->getView()); //Copy of the current view of the renderTexture

	sf::Vector2f viewCentre(currentView.getCenter()); //Centre position of the view 
	sf::Vector2f moveVector(subtractVector(viewCentre, position)); //Vector used to move the view 
	sf::Vector2f movement;
	

	viewCentre.y = position.y;
	viewCentre.x = position.x;

	float viewWidth = currentView.getSize().x;
	float viewHeight = currentView.getSize().y;

	if (viewCentre.x - (viewWidth/2) < 0)
	{
		viewCentre.x = viewWidth / 2;
	}
	//if (viewCentre.x + (viewWidth / 2) > mapWidth)
	//{
	//	viewCentre.x = mapWidth - viewWidth / 2;
	//}
	if (viewCentre.y - (viewHeight / 2) < 0)
	{
		viewCentre.y = viewHeight / 2;
	}
	/*if (viewCentre.y + (viewHeight / 2) > mapHeight)
	{
		viewCentre.y = mapHeight - viewHeight / 2;
	}
*/

	
	float lerp;
	if (isPlayerSprinting)
	{//If the player is moving 
		//translate the camera slowly behind the player
		//move the camera at a fraction of the speed of the player
		lerp = static_cast<float>(0.06);

	}
	else
	{
		lerp = static_cast<float>(0.035);
	}
	sf::Vector2f camPos;
	camPos = currentView.getCenter();
	camPos.x += (position.x - camPos.x) * lerp;
	camPos.y += (position.y - camPos.y) * lerp;
	currentView.setCenter(camPos);
	renderTexture_->setView(currentView);
	//else
	//{
	//	//If the player isn't moving 
	//	if (mod_ < 1.f)
	//	{
	//		lerpCam_ = true;
	//		mod_ += (delta.asSeconds() * 0.001f);
	//	}
	//	else
	//	{
	//		mod_ = 0.f;
	//		lerpCam_ = false;
	//	}

	//	if (lerpCam_)
	//	{//If we want to linearly interpolate the cameras position
	//		//to smoothly translate it to the player

	//		float posX = viewCentre.x + (position.x - viewCentre.x) * mod_;
	//		float posY = viewCentre.y + (position.y - viewCentre.y) * mod_;
	//		viewCentre.x = posX;
	//		viewCentre.y = posY;

	//		currentView.setCenter(viewCentre);
	//		renderTexture_->setView(currentView);
	//		
	//	}
	//}


}