#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "..\States\State.h"
class MenuState :
	public State
{
public:
	MenuState(int, sf::RenderWindow*, sf::RenderTexture*);
	~MenuState();
	bool init();
	void render();
	void update(const sf::Time&);
	void handleEvents(sf::Event&, const sf::Time&);
	void deinit();
private:
	sf::Texture txtNoHover_; //Texture with no hover

	sf::RectangleShape noHover_;
	enum Background
	{

	};
};
#endif 
