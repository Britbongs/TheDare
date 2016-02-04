#ifndef PLAYSTATE_H
#define PLAYSTATE_H


#include <vector>
#include <assert.h>

#include <SFML\Graphics.hpp>

#include "GameStates.h"

#include "..\Map\TiledMap.h"
#include "..\Constants\Constants.h"

#include "..\Utils\Camera.h"
#include "..\Utils\Utils.h"
#include "..\Utils\MTileMap.h"

#include "..\Entities\Player.h"
#include "..\Entities\Bullet.h"
#include "..\Entities\Enemy.h"

#include "..\Lights\Light.h"

#include "..\Spawns\Spawner.h"
#include "..\Objects\Objects.h"
#include "..\Spawns\Trigger.h"


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
	void reset();
	bool isCollision(const sf::FloatRect&, const sf::FloatRect&);
	void deinit();
private:
	bool loadTextures();
	void loadStaticLights();
	void setupPlayerSpotlight();
	bool setupRenderTextures();
	bool loadShaderFromFile();
	bool setupPlayer();
	void drawLights();
	void setShaderParam(float, float, float, float);
	void drawScene();
	bool setupEntities();
	bool setupText();
	bool setupInteractables();
	void setupTriggers();
	void handleTrigger();
private:
	struct Lights
	{
		sf::RectangleShape shape;
	};

	MTileMap tmxMap_;
	TiledMap tiledMap_;
	Player player_;
	vector<Enemy> enemies_;
	vector<sf::Vector2f> enemyCentrePos_;
	vector<Spawner> spawners_;
	vector<Trigger> triggers_;
	Camera* camera_;
	Bullet bullets_[gconsts::Gameplay::MAXBULLETS];
	vector<Objects> objects_;

	vector<Lights> lights_;
	vector<Light> lightList_;

	sf::Texture texture_;
	sf::Texture bulletTexture_;
	sf::Texture pointLightTexture_;
	sf::Texture wallLightTexture_;

	sf::RenderTexture lightRenderTxt_;
	sf::RenderTexture sceneRender_;

	sf::RenderStates shaderState_;

	sf::RectangleShape light_;

	sf::Shader shader_;

	sf::Vector2f playerStart_;
	sf::Vector2f mouseWorldPos_;

	sf::Clock reloadClock;
	sf::Time reloadTimer;

	sf::Text ammo_;
	sf::Text reloading_;
	sf::Text gameOverTxt_;
	sf::Text subGameOverTxt_;
	sf::Text pickupTxt_;
	sf::Font font_;

	float reloadTime;

	int clip;
	int maxAmmo;
	int clipUsed;
	int bulletIndex;
	int id;
	int interactableID;

	bool canShoot, clockStarted;
	bool gameOver;
	bool renderPickupTxt;
	bool gunPickedup;

	enum WEAPONS {
		PUNCH = 0, PISTOL = 1
	} weaponSelected;
};
#endif
