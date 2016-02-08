#include "Audio.h"

bool Audio::setBuffer(const std::string path)
{
	if (!buffer_.loadFromFile("res\\audio\\" + path))
		return(false);
	return (true);
}

void Audio::setSoundToBuffer()
{
	sound_.setBuffer(buffer_);
}

void Audio::play()
{
	sound_.play();
}

void Audio::stop()
{
	sound_.stop();
}