#include "Entity.h"

Entity::Entity()
	: vertices_(sf::Quads, 4), texture_(nullptr), animation_(nullptr), loopAnimation_(false), paused_(true), isAnimated_(false)
{
	vertices_[0].position = sf::Vector2f(0.f, 0.f);
	vertices_[1].position = sf::Vector2f(1.f, 0.f);
	vertices_[2].position = sf::Vector2f(1.f, 1.f);
	vertices_[3].position = sf::Vector2f(0.f, 1.f);
}

void Entity::setVertexLocalPosition(int index, sf::Vector2f position)
{
	assert(index < vertices_.getVertexCount() && index >= 0);
	vertices_[index].position = position;
}

void Entity::setVertexColour(int index, sf::Color colour)
{
	assert(index < vertices_.getVertexCount() && index >= 0);
	vertices_[index].color = colour;
}

void Entity::setVertexTextureCoords(int index, sf::Vector2f position)
{
	assert(index < vertices_.getVertexCount() && index >= 0);
	vertices_[index].texCoords = position;
}

void Entity::setTexture(sf::Texture* p_txt)
{
	texture_ = p_txt;
}

void Entity::setMap(TiledMap* p_map)
{
	p_tileMap_ = p_map;
}

void Entity::setID(const int nid)
{
	id = nid;
}

void Entity::resizeArray(int size)
{
	vertices_.resize(size);
}

void Entity::setPrimitiveType(sf::PrimitiveType a)
{
	vertices_.setPrimitiveType(a);
}

void Entity::setAlive(bool state)
{
	alive = state;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (texture_)
		states.texture = texture_;

	target.draw(vertices_, states);
}

sf::FloatRect Entity::getLocalBounds() const
{
	sf::FloatRect localBounds;

	localBounds.left = vertices_[0].position.x;
	localBounds.top = vertices_[0].position.y;
	localBounds.width = 1.f;
	localBounds.height = 1.f;


	return(localBounds);
}

sf::FloatRect Entity::getGlobalBounds() const
{
	return (getTransform().transformRect(getLocalBounds()));
}

void Entity::setAnimation(const Animation& a)
{
	animation_ = &a;
	texture_ = animation_->getSpriteSheet();
}

void Entity::isAnimated(bool b)
{
	isAnimated_ = b;

}

void Entity::updateAnimation(const sf::Time& delta)
{
	if (!paused_ && animation_)
	{//if not paused and the animation isn't null

		animationTimer_ += delta;

		if (animationTimer_ > frameTime_)
		{//if the time in the current frame > frame time
			animationTimer_ = sf::microseconds(animationTimer_.asMicroseconds() % frameTime_.asMicroseconds());

			if (currentFrame_ + 1 < animation_->getSize())
				++currentFrame_;
			else
			{
				currentFrame_ = 0;

				if (!loopAnimation_)
				{
					paused_ = true;
				}
			}
		}
		setFrame(currentFrame_, false);
	}
}

void Entity::setFrameTime(const sf::Time& frameTime)
{
	frameTime_ = frameTime;
}

bool Entity::init()
{
	return(true);
}

void Entity::setFrame(int index, bool reset)
{
	if (animation_)
	{

		sf::IntRect rect = animation_->getFrame(index);
		float left, top, width, height;
		left = static_cast<float>(rect.left);
		top = static_cast<float>(rect.top);
		width = static_cast<float>(rect.width);
		height = static_cast<float>(rect.height);
		
		vertices_[0].position = sf::Vector2f(0.f, 0.f);
		vertices_[1].position = sf::Vector2f(1.f, 0.f);
		vertices_[2].position = sf::Vector2f(1.f, 1.f);
		vertices_[3].position = sf::Vector2f(0.f, 1.f);

		vertices_[0].texCoords = sf::Vector2f(left, top);
		vertices_[1].texCoords = sf::Vector2f(left + width, top);
		vertices_[2].texCoords = sf::Vector2f(left + width, top + height);
		vertices_[3].texCoords = sf::Vector2f(left, top + height);

	}
	if (reset)
		animationTimer_ = sf::Time::Zero;
}

void Entity::pauseAnimation()
{
	paused_ = true;
}

void Entity::playAnimation()
{
	paused_ = false;
}

void Entity::stopAnimation()
{
	currentFrame_ = 0;
	animationTimer_ = sf::Time::Zero;
}

int Entity::getVertexCount() const
{
	return(vertices_.getVertexCount());
}

void Entity::setAnimationLoop(bool a)
{
	loopAnimation_ = a;
}