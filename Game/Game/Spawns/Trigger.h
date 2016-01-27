#ifndef TRIGGER_H
#define TRIGGER_H

#include "Spawner.h"

class Trigger
{
public: 
	Trigger(Spawner*, sf::Vector2f, sf::Vector2f);
	
	void activate();
	void decactivate();
	
	bool isInTrigger(sf::Vector2f) const;
	bool hasBeenTriggered() const;

private:
	Spawner* const spawner_;
	const sf::FloatRect rect_;
	bool triggered_;
};
#endif
