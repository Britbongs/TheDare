
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML\Graphics.hpp> 
#include <string> 

#define FREE '0'
#define BLOCKED '1'
#define ENEMY '2'

namespace gconsts
{
	namespace Window
	{
		static std::string SCREEN_TITLE("The Dare - INDEV");
		static const int WIDTH(1024);
		static const int HEIGHT(640);
	}

	namespace States
	{
		const int GAMEPLAY_STATE_ID(1);
	}

	namespace Gameplay
	{
		const int TILESIZE(64);
		const int MAXBULLETS(6);
		//const int START_AMMO(12);
		const int START_AMMO(999);
		const int MAXENEMIES(2);
	}

	namespace Assets
	{
		const std::string MAP_LOCATION("res//test.scr");
		const std::string FONT_LOCATION("res//fonts//seriphim.ttf");
		const std::string LIGHT_LAYER("Lights");
	}
}

#endif