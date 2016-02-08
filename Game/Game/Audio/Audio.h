#ifndef AUDIO_H
#define AUDIO_H

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <string>

class Audio
{
public:
	sf::Sound getSound() const { return sound_; }

	bool setBuffer(const std::string);
	void setSoundToBuffer();
	void play();
	void stop();
	sf::Sound sound_;
private:
	sf::SoundBuffer buffer_;
};


#endif
