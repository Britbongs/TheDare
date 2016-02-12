#ifndef LIGHTS
#define LIGHTS
#include <SFML\Graphics.hpp> 
#include "..\Constants\Constants.h"

class Light
{
public: 
	
	Light(int, int, const sf::Texture&, const bool, const bool, const float, const int, const int, const int, const int);

	bool getFlicker() const { return flicker_; }
	bool getLit() const { return lit_; }
	bool getBroke() const { return broke_; }
	bool getOffOnCollide() const { return offOnCollide_; }

	sf::RectangleShape getShape() const { return shape_; }

	void setLit(const bool state) { lit_ = state; }
	void setBroke(const bool state) { broke_ = state; }

	void flicker();
	void setScale(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void render(sf::RenderTexture&) const;
	void setColour(const sf::Color);
	void reset();

private://Functions
	void setupOrientation(); //rotate the shape to the right orientation

private: //Data memebers
	const int type_;
	int orientation_;
	float flickerTime_;

	bool flicker_;
	bool lit_;
	bool broke_;
	bool offOnCollide_;

	sf::RectangleShape shape_;

	sf::Clock timer_;
	sf::Time time_;
};
#endif