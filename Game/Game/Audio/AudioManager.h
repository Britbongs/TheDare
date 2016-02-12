#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <assert.h>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>

class AudioManager
{
public:
	AudioManager();
	~AudioManager();
	static AudioManager *Get();

	sf::SoundBuffer buffers_[20];

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
		PLAYER_HURT = 9,
		GLASS_SMASH = 10,
		BREATH_OF_DEATH = 11,
		DEATH_IS_COMING = 12,
		EVIL_LAUGH = 13,
		PANIC_SCARE = 14,
		SCARY_VIOLINS = 15,
		SCREAM_WOMAN = 16,
		SCREAM_ZOMBIE = 17,
		WHAT_ARE_YOU_DOING_HERE = 18

	};


private:
	static AudioManager* spAMgr;

	bool loadSoundsIntoBuffer();
};

#endif // !AUDIOMANAGER_H

