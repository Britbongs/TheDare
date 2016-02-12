#include "Audio.h"

bool Audio::loadSoundsIntoBuffers()
{
	std::string root("res\\audio\\");
	if (!buffers_[0].loadFromFile(root + "background.ogg"))
		return (false);
	if (!buffers_[1].loadFromFile(root + "backgroundRain.ogg"))
		return (false);
	if (!buffers_[2].loadFromFile(root + "enemyDeath.ogg"))
		return (false);
	if (!buffers_[3].loadFromFile(root + "enemyHurt.ogg"))
		return (false);
	if (!buffers_[4].loadFromFile(root + "footstep.wav"))
		return (false);
	if (!buffers_[5].loadFromFile(root + "gunPickup.ogg"))
		return (false);
	if (!buffers_[6].loadFromFile(root + "gunshot.ogg"))
		return (false);
	if (!buffers_[7].loadFromFile(root + "notePickup.ogg"))
		return (false);
	if (!buffers_[8].loadFromFile(root + "playerDeath.ogg"))
		return (false);
	if (!buffers_[9].loadFromFile(root + "playerHurt.ogg"))
		return (false);
	return(true);
}