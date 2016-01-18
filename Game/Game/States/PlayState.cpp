#include "PlayState.h"

PlayState::PlayState(int STATE_ID, sf::RenderWindow* window, sf::RenderTexture* renderTexture) :
State(STATE_ID, window, renderTexture), bulletIndex(0), clip(gconsts::Gameplay::MAXBULLETS), maxAmmo(gconsts::Gameplay::START_AMMO), clipUsed(0), canShoot(true), clockStarted(false), reloadTime(1.5f)
, enemies_(gconsts::Gameplay::MAXENEMIES), enemyCentrePos_(gconsts::Gameplay::MAXENEMIES)
{
}

PlayState::~PlayState()
{//Deleting class pointers
	deinit();
}

bool PlayState::init()
{
	if (!tmxMap_.loadMap("res//levels//level1.tmx"))
		return(false);

	if (!texture_.loadFromFile("res//entities//player.png"))
		return(false);
	if (!bulletTexture_.loadFromFile("res//entities//bulletsprite.png"))
		return(false);

	tiledMap_.setTMXFile(&tmxMap_);
	tiledMap_.setPointers(&player_, enemies_);
	tiledMap_.initaliseMap();

	player_.setMap(&tiledMap_);
	player_.setScale(64.f, 64.f);
	player_.setPosition(4 * 64, 8 * 64);

	player_.setID(0);

	if (!player_.init())
		return(false);

	for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
	{
		enemies_[i].setMap(&tiledMap_);
		enemies_[i].setScale(64.f, 64.f);
		enemies_[i].setAlive(false);
		enemies_[i].setID(i + 1);
		if (!enemies_[i].init())
		{
			return (false);
		}
	}

	enemies_[0].setPosition(24 * 64, 12 * 64);
	enemies_[1].setPosition(28 * 64, 12 * 64);


	for (int i(0); i < gconsts::Gameplay::MAXBULLETS; i++)
	{
		bullets_[i].setPosition(0, 0);
		bullets_[i].setRotation(0);
		bullets_[i].setScale(8, 3);
		bullets_[i].setMap(&tiledMap_);
		bullets_[i].setTexture(&bulletTexture_);
		bullets_[i].setVertexTextureCoords(0, sf::Vector2f(0.f, 0.f));
		bullets_[i].setVertexTextureCoords(1, sf::Vector2f(8.f, 0.f));
		bullets_[i].setVertexTextureCoords(2, sf::Vector2f(8.f, 3.f));
		bullets_[i].setVertexTextureCoords(3, sf::Vector2f(0.f, 3.f));
		bullets_[i].setAlive(false);
	}

	if (!font_.loadFromFile("res//fonts//Seriphim.ttf"))
	{
		return(false);
	}

	reloading_.setFont(font_);
	ammo_.setFont(font_);
	gameOverTxt_.setFont(font_);
	subGameOverTxt_.setFont(font_);

	ammo_.setColor(sf::Color::Green);
	reloading_.setColor(sf::Color::Red);
	gameOverTxt_.setColor(sf::Color::Red);
	subGameOverTxt_.setColor(sf::Color::Red);


	ammo_.setCharacterSize(16);
	reloading_.setCharacterSize(16);
	gameOverTxt_.setCharacterSize(128);
	subGameOverTxt_.setCharacterSize(64);


	reloading_.setString("Reloading...");
	gameOverTxt_.setString("Game Over ...");
	subGameOverTxt_.setString("Press P to play again");



	camera_ = new Camera(sf::Vector2u(tmxMap_.getLayer()[0]->width, tmxMap_.getLayer()[0]->width), renderTexture_);
	sf::View v(renderTexture_->getView());
	//v.setCenter(tmxMap_.getWidth() * tmxMap_.getTileWidth(), tmxMap_.getHeight() * tmxMap_.getTileHeight());
	v.zoom(0.75f);
	renderTexture_->setView(v);

	{//Loading lights & shaders
		//Load the lightmask texture
		if (!pointLightTexture_.loadFromFile(gconsts::Assets::POINT_LIGHT_TEXTURE))
			return(false);

		if (!wallLightTexture_.loadFromFile(gconsts::Assets::WALL_LIGHT_TEXTURE))
			return(false);

		//Create a RenderTexture to draw the lights onto
		if (!lightRenderTxt_.create(renderTexture_->getSize().x, renderTexture_->getSize().y))
			return(false);

		if (!sceneRender_.create(tmxMap_.getTileWidth() * tmxMap_.getWidth(), tmxMap_.getTileHeight() * tmxMap_.getHeight()))
			return(false);

		//Initialising the light RectangleShape
		light_.setTexture(&pointLightTexture_);
		light_.setSize(sf::Vector2f(static_cast<float>(pointLightTexture_.getSize().x), static_cast<float>(pointLightTexture_.getSize().y)));
		light_.setScale(1.5f, 1.5f);
		setupSceneLights();
		if (!shader_.loadFromFile("res///shaders//vertexShader.vert", "res//shaders//fragmentShader.frag"))
			return(false);


		//Set the renderstates to use the correct shader & blend mode
		shaderState_.shader = &shader_;
		shaderState_.blendMode = sf::BlendAlpha;
	}
	id = 0;
	MObjectGroup obj = tmxMap_.getObjectGroup(0);

	objects_.resize(obj.objects.size());

	for (int i(0); i < static_cast<int> (objects_.size()); ++i)
	{
		objects_[i].setPosition(obj.objects[i].x, obj.objects[i].y);
		objects_[i].setSize(sf::Vector2f(64, 64));
		objects_[i].setTexture(&dirtyBed_);
	}

	setupEntitiyPositions();
	return(true);
}

void PlayState::render()
{

	drawLights(); //draw lights to a texture

	if (!gameOver)
	{
		setShaderParam(0.27f, 0.15f, 0.3f, 0.2f); //set the paramters of a shader

		drawScene(); //draw the scene to a texture

		sf::Sprite s(sceneRender_.getTexture());
		renderTexture_->draw(s, shaderState_);
		renderTexture_->draw(ammo_);
		renderTexture_->draw(player_.getSprintRect());
		renderTexture_->draw(player_.getHealthRect());

		if (!canShoot && maxAmmo > 0)
		{
			renderTexture_->draw(reloading_);
		}
	}
	else
	{
		setShaderParam(0.8f, 0.04f, 0.4f, 0.3f);
		drawScene();
		sf::Sprite s(sceneRender_.getTexture());
		renderTexture_->draw(s, shaderState_);
		renderTexture_->draw(gameOverTxt_);
		renderTexture_->draw(subGameOverTxt_);
	}
	renderTexture_->display();
}

void PlayState::update(const sf::Time& delta)
{
	if (!gameOver)
	{

		sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);
		mouseWorldPos_ = renderTexture_->mapPixelToCoords(mousePos);
		sf::Vector2f playerCentrePos(player_.getPosition().x + player_.getGlobalBounds().width / 2, player_.getPosition().y + player_.getGlobalBounds().height / 2);
		sf::Vector2f enemyRot;
		float enemyRotation;
		for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
		{
			if (enemies_[i].getAlive())
			{
				enemyCentrePos_[i].x = enemies_[i].getPosition().x + enemies_[i].getGlobalBounds().width / 2;
				enemyCentrePos_[i].y = enemies_[i].getPosition().y + enemies_[i].getGlobalBounds().height / 2;
				enemyRot = subtractVector(playerCentrePos, enemyCentrePos_[i]);
				enemyRotation = (degrees(atan2(enemyRot.y, enemyRot.x)));
				enemies_[i].update(delta, playerCentrePos, enemyRotation);
			}
		}

		sf::Vector2f playerRot(subtractVector(mouseWorldPos_, player_.getPosition()));
		float playerRotation = (degrees(atan2(playerRot.y, playerRot.x)));


		player_.update(delta, playerRotation, renderTexture_);



		for (int i(0); i < gconsts::Gameplay::MAXBULLETS; i++)
		{
			for (int j(0); j < gconsts::Gameplay::MAXENEMIES; j++)
			{
				if (bullets_[i].getAlive())
				{
					bullets_[i].update(delta);
					if (isCollision(bullets_[i].getCollider(), enemies_[j].getCollider()) && enemies_[j].getAlive())
					{
						enemies_[j].setState(1);
						bullets_[i].setAlive(false);
						enemies_[j].takeDamage(bullets_[i].getDamage());
						if (enemies_[j].getCurrentHealth() <= 0)
						{
							enemies_[j].kill();
						}
					}
				}
			}
		}
		if (!canShoot)
		{
			if (reload())
			{
				if (maxAmmo < 6)
				{
					bulletIndex = gconsts::Gameplay::MAXBULLETS - maxAmmo;
					maxAmmo = 0;
				}
				else
				{
					bulletIndex = 0;
					maxAmmo -= clipUsed;
				}

				clipUsed = 0;
				canShoot = true;
			}
		}
		for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
		{
			if (isCollision(enemies_[i].getChaseBox(), player_.getCollider()))
			{
				enemies_[i].setState(1);
			}
			if (isCollision(enemies_[i].getCollider(), player_.getCollider()) && player_.getCanTakeDamage() && player_.getCurrentHealth() > 0)
			{
				player_.setCanTakeDamage(false);
				player_.takeDamage(enemies_[i].getDamage());
				std::cout << player_.getCurrentHealth() << std::endl;

				if (player_.getCurrentHealth() <= 0)
				{
					player_.setAlive(false);
					gameOver = true;
				}
				clipUsed = 0;
				canShoot = true;
			}
		}

		if (!player_.getCanTakeDamage())
		{
			if (player_.invincibility())
			{
				player_.setCanTakeDamage(true);
			}
		}
		light_.setPosition(player_.getPosition().x - light_.getGlobalBounds().width / 2, player_.getPosition().y - light_.getGlobalBounds().height / 2);
		camera_->update(delta, player_.getPosition(), true);

		//GUI TEXT
		clip = gconsts::Gameplay::MAXBULLETS - bulletIndex;
		ammo_.setString("Ammo : " + to_string(clip) + " / " + to_string(maxAmmo));
		ammo_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(10, 30)));
		reloading_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(10, 50)));
	}
	else
	{
		gameOverTxt_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(window_->getSize().x / 4, window_->getSize().y / 4)));
		subGameOverTxt_.setPosition(gameOverTxt_.getPosition().x + 8, gameOverTxt_.getPosition().y + 128);
	}
}

void PlayState::handleEvents(sf::Event& evnt, const sf::Time& delta)
{
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (canShoot && (clip > 0 && maxAmmo >= 0))
		{


			if (evnt.key.code == sf::Mouse::Left)
			{
				bullets_[bulletIndex].setAlive(true);
				sf::Vector2f rot(subtractVector(mouseWorldPos_, player_.getPosition()));
				bullets_[bulletIndex].init(rot, player_.getPosition());
				bulletIndex++;
				clipUsed++;
				if (bulletIndex > gconsts::Gameplay::MAXBULLETS - 1)
				{
					canShoot = false;
				}
			}

		}
		else
		{
			canShoot = false;
		}
	}
	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::R && maxAmmo > 0)
		{
			canShoot = false;
		}

	}
	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::P && gameOver)
		{
			reset();
		}
	}
}

bool PlayState::reload()
{
	if (canShoot == false)
	{
		if (clockStarted == false)
		{
			clockStarted = true;

			reloadClock.restart();
		}
		reloadTimer = reloadClock.getElapsedTime();
		if (reloadTimer.asSeconds() >= reloadTime)
		{
			canShoot = true;
			clockStarted = false;
			return true;
		}
	}
	return false;
}

void PlayState::reset()
{
	gameOver = false;

	player_.setPosition(6 * 64, 6 * 64);
	player_.setAlive(true);
	player_.resetHealth();


	for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
	{
		enemies_[i].setAlive(true);
		enemies_[i].setState(0);
		enemies_[i].resetHealth();
	}

	//enemies_[0].setPosition(24 * 64, 12 * 64);
	//enemies_[1].setPosition(28 * 64, 12 * 64);

	for (int i(0); i < gconsts::Gameplay::MAXBULLETS; i++)
	{
		bullets_[i].setPosition(0, 0);
		bullets_[i].setRotation(0);
		bullets_[i].setAlive(false);
	}

	bulletIndex = 0;
	clip = gconsts::Gameplay::MAXBULLETS;
	maxAmmo = 12;
	canShoot = true;
	clockStarted = false;
}

bool PlayState::isCollision(const sf::FloatRect& a, const sf::FloatRect& b)
{
	if (a.intersects(b))
	{
		return true;
	}
	return false;
}

void PlayState::deinit()
{
	delete camera_;
}


void PlayState::drawLights()
{
	lightRenderTxt_.clear();
	lightRenderTxt_.setView(renderTexture_->getView());
	//for (int i(0); i < lights_.size(); ++i)
	//lightRenderTxt_.draw(lights_[i].shape);
	for (int i(0); i < lightList_.size(); ++i)
		lightList_[i].render(lightRenderTxt_);
	light_.setOrigin(0.5, 0.5f);
	lightRenderTxt_.draw(light_, sf::BlendAdd);
	light_.setOrigin(0.f, 0.f);
	lightRenderTxt_.display();
}

void PlayState::setShaderParam(float r, float g, float b, float intensity)
{
	shader_.setParameter("lightMapTexture", lightRenderTxt_.getTexture());
	shader_.setParameter("resolution", static_cast<float> (renderTexture_->getSize().x), static_cast<float> (renderTexture_->getSize().y));

	shader_.setParameter("ambientColour", r, g, b, intensity);


}

void PlayState::drawScene()
{
	sceneRender_.clear(sf::Color::Black);
	sceneRender_.draw(tiledMap_);

	for (int i(0); i < gconsts::Gameplay::MAXBULLETS; i++)
	{
		if (bullets_[i].getAlive())
		{
			sceneRender_.draw(bullets_[i]);
		}
	}
	player_.setOrigin(0.5, 0.5f);
	if (player_.getAlive())
	{
		sceneRender_.draw(player_);
	}
	player_.setOrigin(0.f, 0.f);
	for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
	{
		if (enemies_[i].getAlive())
		{
			enemies_[i].setOrigin(0.5f, 0.5f);
			sceneRender_.draw(enemies_[i]);
			sceneRender_.draw(enemies_[i].getHealthRect());
			enemies_[i].setOrigin(0.f, 0.f);
		}

	}
	for (int i(0); i < objects_.size(); ++i)
		sceneRender_.draw(objects_[i]);
	sceneRender_.display();
}

void PlayState::setupSceneLights()
{
	MObjectGroup lightGroup; //Object group of lights
	int counter(0);
	bool found(false);
	int ID;

	while (!found && counter < tmxMap_.getObjectGroupCount())
	{
		if (tmxMap_.getObjectGroup(counter).name == gconsts::Assets::LIGHT_LAYER)
		{
			lightGroup = tmxMap_.getObjectGroup(counter);
			found = true;
		}
		++counter;
	}

	//lightList_.resize(lightGroup.objects.size()); 

	found = false;
	counter = 0;

	std::istringstream stream;

	for (int i(0); i < lightGroup.objects.size(); ++i)
	{
		int type(0);
		int orientation(-1);

		sf::Vector2f position(lightGroup.objects[i].x, lightGroup.objects[i].y);

		for (int j(0); j < lightGroup.objects[i].properties.size(); ++j)
		{
			if (lightGroup.objects[i].properties[j].name == "Light")
			{
				stream.clear();
				stream.str(lightGroup.objects[i].properties[j].value);
				stream >> type;

			}
			if (lightGroup.objects[i].properties[j].name == "Orientation")
			{
				stream.clear();
				stream.str(lightGroup.objects[i].properties[j].value);
				stream >> orientation;
				std::cout << "orientation: " << lightGroup.objects[i].properties[j].value << " - " << orientation << std::endl;
			}
		}
		lightList_.push_back(Light(type, orientation, type == 0 ? pointLightTexture_ : wallLightTexture_));
		lightList_[lightList_.size() - 1].setPosition(position);
	}
}

void PlayState::setupEntitiyPositions()
{

	assert(enemies_.size() > 0);
	MObjectGroup entityGroup; //Object group of enemies
	int counter(0);
	bool found(false);
	int ID;

	while (!found && counter < tmxMap_.getObjectGroupCount())
	{//loop through all object groups and find the group 
		if (tmxMap_.getObjectGroup(counter).name == gconsts::Assets::SPAWN_LAYER)
		{
			entityGroup = tmxMap_.getObjectGroup(counter);
			found = true;
		}
		++counter;
	}
	std::istringstream stream;
	counter = 0;

	for (int i(0); i < entityGroup.objects.size(); ++i)
	{
		for (int j(0); j < entityGroup.objects[i].properties.size(); ++j)
		{
			int value;
			if (entityGroup.objects[i].properties[j].name == "Entity")
			{
				stream.clear();
				stream.str(entityGroup.objects[i].properties[j].value);
				stream >> value;
				switch (value)
				{
				case 0:
					player_.setPosition(entityGroup.objects[i].x, entityGroup.objects[i].y);
					break;
				case 1:
					enemies_[counter].setPosition(entityGroup.objects[i].x, entityGroup.objects[i].y);
					enemies_[counter].setAlive(true);
					++counter;
					break;

				}
			}
		}
	}
}