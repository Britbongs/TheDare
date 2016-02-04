
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
		const int POINT_LIGHT(0);
		const int WALL_LIGHT(1);
		const int TILESIZE(64);
		const int MAXBULLETS(6);
		const int START_AMMO(12);
		const int MAXENEMIES(40);
		const int MAXSPAWNS(10);
		//const int START_AMMO(999);
	}

	namespace Assets
	{
		const std::string FONT_LOCATION("res//fonts//seriphim.ttf");
		const std::string LIGHT_LAYER("Lights");
		const std::string SPAWN_LAYER("Spawns");
		const std::string INTERACTABLE_LAYER("Interactables");
		const std::string TRIGGERS_LAYER("Triggers");
		const std::string WALL_LIGHT_TEXTURE("res//lights//wall_light.png");
		const std::string POINT_LIGHT_TEXTURE("res//lights//lightmask.png");
		const std::string SPOT_LIGHT_TEXTURE("res//lights//torch_light.png");

	}
}

#endif