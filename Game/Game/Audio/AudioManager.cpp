#include "AudioManager.h"
AudioManager *AudioManager::spAMgr(nullptr);

AudioManager::AudioManager()
{
	assert(spAMgr == nullptr);
	assert(loadSoundsIntoBuffer());
	spAMgr = this;
}

AudioManager::~AudioManager()
{
	spAMgr = NULL;
}

AudioManager * AudioManager::Get()
{
	assert(spAMgr);
	return spAMgr;
}

bool AudioManager::loadSoundsIntoBuffer()
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
	if (!buffers_[10].loadFromFile(root + "glassSmash.ogg"))
		return (false);
	if (!buffers_[11].loadFromFile(root + "breathOfDeath.ogg"))
		return (false);
	if (!buffers_[12].loadFromFile(root + "deathIsComing.ogg"))
		return (false);
	if (!buffers_[13].loadFromFile(root + "evilLaugh.ogg"))
		return (false);
	if (!buffers_[14].loadFromFile(root + "panicScare.ogg"))
		return (false);
	if (!buffers_[15].loadFromFile(root + "scaryViolins.ogg"))
		return (false);
	if (!buffers_[16].loadFromFile(root + "screamWoman.ogg"))
		return (false);
	if (!buffers_[17].loadFromFile(root + "screamZombie.ogg"))
		return (false);
	if (!buffers_[18].loadFromFile(root + "whatAreYouDoingHere.ogg"))
		return (false);
	return(true);
}
