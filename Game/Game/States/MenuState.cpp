#include "MenuState.h"



MenuState::MenuState(int stateID, sf::RenderWindow* window, sf::RenderTexture* texture)
	: State(stateID, window, texture)
{
}


MenuState::~MenuState()
{
}

bool MenuState::init()
{
	if (!txtNoHover_.loadFromFile("res//illustrations//menu_no_hover.png"))
		return(false);
	txtNoHover_.setSmooth(true);
	noHover_.setSize(sf::Vector2f(window_->getSize().x, window_->getSize().y));
	noHover_.setScale(1, 1);
	noHover_.setPosition(0,0);
	noHover_.setTexture(&txtNoHover_);

	return (true);
}

void MenuState::render()
{
	renderTexture_->draw(noHover_);
}

void MenuState::update(const sf::Time& delta)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		stateCompleted_ = true;
}

void MenuState::handleEvents(sf::Event &, const sf::Time &)
{

}

void MenuState::deinit()
{

}
