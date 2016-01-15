#ifndef PLAYSTATE_H
#define PLAYSTATE_H


#include <vector>
#include <assert.h>

#include <SFML\Graphics.hpp>

#include "GameStates.h"

#include "..\Map\TiledMap.h"
#include "..\Utils\MTileMap.h"
#include "..\Constants\Constants.h"
#include "..\Utils\Camera.h"
#include "..\Utils\Utils.h"
#include "..\Entities\Player.h"
#include "..\Entities\Bullet.h"
#include "..\Entities\Enemy.h"
#include "..\Lights\Light.h"

class PlayState :
	public State
{
public:
	PlayState(int, sf::RenderWindow*, sf::RenderTexture*);//Param: stateID
	~PlayState();
	bool init();
	void render();
	void update(const sf::Time&); 
	void handleEvents(sf::Event&, const sf::Time&);
	bool reload();
	bool isCollision(const sf::FloatRect&, const sf::FloatRect&);
	void deinit();
private: 
	void setupSceneLights();
	void drawLights();
	void setShaderParam(float,float,float,float);
	void drawScene();
private: 
	struct Lights
	{
		sf::RectangleShape shape;
	};

	MTileMap tmxMap_;
	TiledMap tiledMap_;
	Player player_;
	Enemy enemy_;
	Camera* camera_;
	Bullet bullets_[gconsts::Gameplay::MAXBULLETS];

	std::vector<sf::RectangleShape> objects_;
	std::vector<Lights> lights_;
	std::vector<Light> lightList_;

	sf::Texture texture_;
	sf::Texture bulletTexture_;
	sf::Texture pointLightTexture_;
	sf::Texture wallLightTexture_;
	sf::Texture dirtyBed_;
	
	sf::RenderTexture lightRenderTxt_;
	sf::RenderTexture sceneRender_;
	
	sf::RenderStates shaderState_;
	
	sf::RectangleShape light_;

	sf::Shader shader_;

	sf::Vector2f mouseWorldPos_;
	
	sf::Clock reloadClock;
	sf::Time reloadTimer;

	sf::Text ammo_;
	sf::Text reloading_;
	sf::Font font_;
	
	float reloadTime;

	int clip;
	int maxAmmo;
	int clipUsed;
	int bulletIndex;
	int id;
	
	bool canShoot, clockStarted;
};
#endif