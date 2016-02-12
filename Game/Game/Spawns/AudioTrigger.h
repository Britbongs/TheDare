#ifndef AUDIOTRIGGER_H
#define AUDIOTRIGGER_H

#include <SFML\Graphics.hpp>

#include "..\Audio\AudioManager.h"

class AudioTrigger
{
public:
	AudioTrigger(const int, sf::Vector2f, sf::Vector2f);//param : file path, position, size
	
	void activate();
	void reset();

	bool isInTrigger(sf::Vector2f);

private:

	sf::FloatRect rect_;
	bool triggered_;
	bool playing_;
	AudioManager* aManage_;
	sf::Sound sound_;
private:
};

#endif
