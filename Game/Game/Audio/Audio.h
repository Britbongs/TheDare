#ifndef AUDIO_H
#define AUDIO_H

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <string>

class Audio
{
public:
	
	bool loadSoundsIntoBuffers();

	sf::SoundBuffer buffers_[12];

	enum SOUNDARRAY
	{
		BACKGROUND = 0,
		BACKGROUND_RAIN = 1,
		ENEMY_DEATH = 2,
		ENEMY_HURT = 3,
		FOOTSTEP = 4,
		GUNPICKUP = 5,
		GUNSHOT = 6,
		NOTE_PICKUP = 7,
		PLAYER_DEATH = 8,
		PLAYER_HURT = 9
	};
private:
};


#endif
