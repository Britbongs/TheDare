#ifndef ENTITY_H
#define ENTITY_H

#include <SFML\Graphics.hpp> 
#include <assert.h>
#include "..\Utils\VectorUtils.h"
#include "..\Utils\Utils.h"
#include "..\Map\TiledMap.h"
#include "..\Utils\AnimatedSprite.hpp"
#include "..\Utils\Animation.hpp"

class Entity
	: public sf::Drawable, public sf::Transformable
{
public:
	Entity();
	void setVertexLocalPosition(int, sf::Vector2f);
	void setVertexColour(int, sf::Color);
	void setVertexTextureCoords(int, sf::Vector2f);
	void setTexture(sf::Texture*);
	void setMap(TiledMap*);
	void resizeArray(int);
	void setPrimitiveType(sf::PrimitiveType);
	void setAlive(const bool);
	void setID(const int);
	int getVertexCount() const;
	sf::Vector2f getVertexGlobalPosition(int) const;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	sf::FloatRect getCollider() const { return collider_; }
	int getID() const { return id; }
	bool getAlive() const { return alive; }
	void setAnimation(const Animation&);
	void isAnimated(bool);
	void updateAnimation(const sf::Time&);
	void setFrameTime(const sf::Time&);
	void setFrame(int, bool = true);
	void playAnimation();
	void pauseAnimation();
	void stopAnimation();
	virtual bool init();
	void setAnimationLoop(bool);
private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
private:
	sf::VertexArray vertices_;
	const sf::Texture* texture_;
	const Animation* animation_;
	sf::Time animationTimer_;
	sf::Time frameTime_;
	float alive;
	int currentFrame_;
	bool isAnimated_;
	bool paused_;
	bool loopAnimation_;
protected:
	TiledMap* p_tileMap_;
	sf::FloatRect collider_;
	int id;

};
#endif