#include "AudioTrigger.h"

AudioTrigger::AudioTrigger(const int id, sf::Vector2f pos, sf::Vector2f size)
	: rect_(pos, size), triggered_(false), playing_(false)
{
	aManage_ = AudioManager::Get();
	sound_.setBuffer(aManage_->buffers_[id]);
}

void AudioTrigger::activate()
{
	if (!triggered_ && playing_)
	{
		playing_ = false;
		triggered_ = true;
		sound_.play();
	}
}

bool AudioTrigger::isInTrigger(sf::Vector2f place)
{
	if (rect_.contains(place))
	{
		playing_ = true;
		return(true);
	}
	else
		return(false);
}

void AudioTrigger::reset()
{
	triggered_ = false;
	playing_ = false;
}
