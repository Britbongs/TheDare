#include "PlayState.h"

PlayState::PlayState(int STATE_ID, sf::RenderWindow* window, sf::RenderTexture* renderTexture) :
	State(STATE_ID, window, renderTexture), bulletIndex(0), clip(gconsts::Gameplay::MAXBULLETS), maxAmmo(gconsts::Gameplay::START_AMMO), clipUsed(0), canShoot(true), clockStarted(false), reloadTime(1.5f)
	, enemies_(gconsts::Gameplay::MAXENEMIES), enemyCentrePos_(gconsts::Gameplay::MAXENEMIES), renderPickupTxt(false), gunPickedup(false), pausedText(false), eManage_(nullptr)
{
}

PlayState::~PlayState()
{//Deleting class pointers
	deinit();
}

bool PlayState::init()
{
	//window_->setMouseCursorVisible(false); //when cross hair is completed
	weaponSelected = PUNCH;

	if (!loadTextures())
		return(false);

	tiledMap_.setTMXFile(&tmxMap_);
	tiledMap_.setPointers(&player_, enemies_);
	tiledMap_.initaliseMap();

	if (!setupPlayer())
		return(false);


	setupEntities();
	setupText();
	setupInteractables();

	setupTriggers();


	for (int i(0); i < gconsts::Gameplay::MAXBULLETS; i++)
	{
		bullets_[i].setPosition(0, 0);
		bullets_[i].setRotation(0);
		bullets_[i].setScale(10, 4);
		bullets_[i].setMap(&tiledMap_);
		bullets_[i].setTexture(&bulletTexture_);
		bullets_[i].setVertexTextureCoords(0, sf::Vector2f(0.f, 0.f));
		bullets_[i].setVertexTextureCoords(1, sf::Vector2f(8.f, 0.f));
		bullets_[i].setVertexTextureCoords(2, sf::Vector2f(8.f, 3.f));
		bullets_[i].setVertexTextureCoords(3, sf::Vector2f(0.f, 3.f));
		bullets_[i].setAlive(false);
	}

	camera_ = new Camera(sf::Vector2u(tmxMap_.getLayer()[0]->width, tmxMap_.getLayer()[0]->width), renderTexture_);
	sf::View v(renderTexture_->getView());

	v.zoom(0.75f);
	renderTexture_->setView(v);


	if (!setupRenderTextures())
		return(false);

	setupPlayerSpotlight();

	loadStaticLights();

	if (!loadShaderFromFile())
		return(false);

	//audio
	backgroundSnd_.setBuffer("background.ogg");
	backgroundSnd_.setSoundToBuffer();
	backgroundSnd_.sound_.setLoop(1);
	backgroundSnd_.sound_.setVolume(25);
	backgroundSnd_.play();

	//textbox
	textBox_.setFillColor(sf::Color::White);
	textBox_.setSize(sf::Vector2f(600, 400));
	textBox_.setScale(sf::Vector2f(1, 1));

	return(true);
}

bool PlayState::loadTextures()
{
	if (!pointLightTexture_.loadFromFile(gconsts::Assets::POINT_LIGHT_TEXTURE))
		return(false);

	if (!wallLightTexture_.loadFromFile(gconsts::Assets::WALL_LIGHT_TEXTURE))
		return(false);

	if (!tmxMap_.loadMap("res//levels//level1.tmx"))
		return(false);

	if (!texture_.loadFromFile("res//entities//player.png"))
		return(false);

	if (!bulletTexture_.loadFromFile("res//entities//bulletsprite.png"))
		return(false);

	if (!font_.loadFromFile("res//fonts//Seriphim.ttf"))
		return(false);


	return(true);
}

bool PlayState::setupPlayer()
{
	player_.setMap(&tiledMap_);
	player_.setScale(64.f, 64.f);
	player_.setPosition(4 * 64, 8 * 64);

	player_.setID(0);

	if (!player_.init())
		return(false);
	return(true);
}

bool PlayState::setupRenderTextures()
{
	if (!lightRenderTxt_.create(renderTexture_->getSize().x, renderTexture_->getSize().y))
		return(false);

	if (!sceneRender_.create(tmxMap_.getTileWidth() * tmxMap_.getWidth(), tmxMap_.getTileHeight() * tmxMap_.getHeight()))
		return(false);

	return(true);
}

bool PlayState::setupText()
{
	reloading_.setFont(font_);
	ammo_.setFont(font_);
	gameOverTxt_.setFont(font_);
	subGameOverTxt_.setFont(font_);
	pickupTxt_.setFont(font_);
	noteTxt_.setFont(font_);


	ammo_.setColor(sf::Color::Green);
	reloading_.setColor(sf::Color::Red);
	gameOverTxt_.setColor(sf::Color::Red);
	subGameOverTxt_.setColor(sf::Color::Red);
	pickupTxt_.setColor(sf::Color::Red);
	noteTxt_.setColor(sf::Color::Black);


	ammo_.setCharacterSize(16);
	reloading_.setCharacterSize(16);
	gameOverTxt_.setCharacterSize(128);
	subGameOverTxt_.setCharacterSize(64);
	pickupTxt_.setCharacterSize(16);
	noteTxt_.setCharacterSize(20);


	reloading_.setString("Reloading...");
	gameOverTxt_.setString("Game Over ...");
	subGameOverTxt_.setString("Press P to play again");
	pickupTxt_.setString("Press E to pick up");
	noteTxt_.setString("");

	return(true);
}

bool PlayState::setupInteractables()
{
	MObjectGroup interactableGroup;
	int counter(0);
	bool found(false);

	while (!found && counter < tmxMap_.getObjectGroupCount())
	{
		if (tmxMap_.getObjectGroup(counter).name == gconsts::Assets::INTERACTABLE_LAYER)
		{
			interactableGroup = tmxMap_.getObjectGroup(counter);
			found = true;
		}
		++counter;
	}
	std::istringstream stream;

	for (int i(0); i < static_cast<int>(interactableGroup.objects.size()); ++i)
	{
		int id(-1);
		sf::String txt("");
		float x, y;
		for (int j(0); j < static_cast<int>(interactableGroup.objects[i].properties.size()); ++j)
		{
			if (interactableGroup.objects[i].properties[j].name == "ID")
			{
				stream.clear();
				stream.str(interactableGroup.objects[i].properties[j].value);
				stream >> id;
				x = static_cast<float>(interactableGroup.objects[i].x);
				y = static_cast<float>(interactableGroup.objects[i].y);
			}
			if (interactableGroup.objects[i].properties[j].name == "Text")
			{
				stream.clear();
				stream.str(interactableGroup.objects[i].properties[j].value);
				txt = stream.str();
				txt.replace("\\n", "\n");
			}
		}
		objects_.push_back(Objects(x, y, id, txt));
	}

	for (int i(0); i < static_cast<int>(objects_.size()); i++)
	{
		if (!objects_[i].initSpritesheet())
		{
			return (false);
		}
	}
	return true;
}

void PlayState::setupPlayerSpotlight()
{
	light_.setTexture(&pointLightTexture_);
	light_.setSize(sf::Vector2f(static_cast<float>(pointLightTexture_.getSize().x), static_cast<float>(pointLightTexture_.getSize().y)));
	light_.setScale(1.2f, 1.2f);
}

bool PlayState::loadShaderFromFile()
{
	if (!shader_.loadFromFile("res///shaders//vertexShader.vert", "res//shaders//fragmentShader.frag"))
		return(false);


	//Set the renderstates to use the correct shader & blend mode
	shaderState_.shader = &shader_;
	shaderState_.blendMode = sf::BlendAlpha;
	return(true);
}

void PlayState::loadStaticLights()
{
	MObjectGroup lightGroup; //Object group of lights
	int counter(0);
	bool found(false);

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

	for (int i(0); i < static_cast<int>(lightGroup.objects.size()); ++i)
	{
		int type(0);
		int orientation(-1);

		sf::Vector2f position(static_cast<float>(lightGroup.objects[i].x), static_cast<float>(lightGroup.objects[i].y));

		for (int j(0); j < static_cast<int>(lightGroup.objects[i].properties.size()); ++j)
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

bool PlayState::setupEntities()
{
	eManage_ = new EMngr(&sceneRender_, &player_, bullets_, &tiledMap_);

	if (!eManage_->init())
		return (false);

	MObjectGroup entityGroup; //Object group of enemies
	int counter(0);
	bool found(false);

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


	for (int i(0); i < static_cast<int>(entityGroup.objects.size()); ++i)
	{
		bool isEntity(false);
		bool triggered(false);
		int entityID(-1);
		int entityCount(0);
		int triggerID(-1);
		for (int j(0); j < static_cast<int>(entityGroup.objects[i].properties.size()); ++j)
		{
			if (entityGroup.objects[i].properties[j].name == "Entity")
			{
				isEntity = true;
				stream.clear();
				stream.str(entityGroup.objects[i].properties[j].value);
				stream >> entityID;
			}

			if (entityGroup.objects[i].properties[j].name == "Count")
			{
				stream.clear();
				stream.str(entityGroup.objects[i].properties[j].value);
				stream >> entityCount;
			}

			if (entityGroup.objects[i].properties[j].name == "Trigger")
			{
				triggered = true;
				stream.clear();
				stream.str(entityGroup.objects[i].properties[j].value);
				stream >> triggerID;
			}

		}
		if (isEntity)
		{//if we found an entity that needs to be positioned
			switch (entityID)
			{
			case 0:
				playerStart_ = sf::Vector2f(static_cast<float>(entityGroup.objects[i].x), static_cast<float>(entityGroup.objects[i].y));
				player_.setPosition(playerStart_);
				break;
			case 1:
				sf::Vector2i pos(static_cast<int> (entityGroup.objects[i].x / gconsts::Gameplay::TILESIZE), static_cast<int>(entityGroup.objects[i].y / gconsts::Gameplay::TILESIZE));
				if (!triggered)
				{
					spawn_.push_back(EnemySpawner(entityCount, pos, &tiledMap_));

					spawn_.back().spawnEnemies();
				}
				else
				{
					spawn_.push_back(EnemySpawner(entityCount, pos, &tiledMap_, true, triggerID));


				}
			}
		}
	}

	return(true);
}

void PlayState::setupTriggers()
{//Accesses trigger data from the tmx map and sets up each location trigger on the map 
	MObjectGroup triggersGroup; //Object group of enemies
	int counter(0);
	bool found(false);

	while (!found && counter < tmxMap_.getObjectGroupCount())
	{//loop through all object groups and find the group 
		if (tmxMap_.getObjectGroup(counter).name == gconsts::Assets::TRIGGERS_LAYER)
		{
			triggersGroup = tmxMap_.getObjectGroup(counter);
			found = true;
		}
		++counter;
	}
	std::istringstream stream;

	for (int i(0); i < static_cast<int>(triggersGroup.objects.size()); ++i)
	{
		int id(-1);
		EnemySpawner* spawner(nullptr);
		for (int j(0); j < static_cast<int>(triggersGroup.objects[i].properties.size()); ++j)
		{
			if (triggersGroup.objects[i].properties[j].name == "ID")
			{
				stream.clear();
				stream.str(triggersGroup.objects[i].properties[j].value);
				stream >> id;
			}
		}

		for (int i(0); i < static_cast<int>(spawn_.size()) && !spawner; ++i)
		{
			if (spawn_[i].isTriggeredSpawner())
				spawner = &spawn_[i];
		}
		sf::Vector2f position(static_cast<float>(triggersGroup.objects[i].x), static_cast<float>(triggersGroup.objects[i].y));
		sf::Vector2f size(static_cast<float>(triggersGroup.objects[i].width), static_cast<float>(triggersGroup.objects[i].height));
		triggers_.push_back(Trigger(spawner, position, size));
	}
}

void PlayState::handleTrigger()
{
	for (int i(0); i < static_cast<int>(triggers_.size()); ++i)
	{
		if (!triggers_[i].hasBeenTriggered())
		{
			if (triggers_[i].isInTrigger(player_.getPosition()))
			{
				triggers_[i].activate();
				std::cout << "ACTIVATED TRIGGER" << std::endl;
			}
		}
	}
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


		if (renderPickupTxt)
		{
			//pickupTxt_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(10,50)));
			pickupTxt_.setPosition((player_.getPosition().x - (player_.getCollider().width / 2)) - 16, (player_.getPosition().y - (player_.getCollider().height / 2)) - 20);
			renderTexture_->draw(pickupTxt_);
		}
		if (pausedText)
		{
			renderTexture_->draw(textBox_);
			renderTexture_->draw(noteTxt_);
		}

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
		if (!pausedText)
		{

			sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);
			mouseWorldPos_ = renderTexture_->mapPixelToCoords(mousePos);


			sf::Vector2f playerRot(subtractVector(mouseWorldPos_, player_.getPosition()));
			float playerRotation = (degrees(atan2(playerRot.y, playerRot.x)));
			player_.update(delta, playerRot, renderTexture_);
			player_.punchTimer();

			eManage_->update(delta);


			bool found(false);
			for (int i(0); i < static_cast<int>(objects_.size()); i++)
			{
				if (isCollision(player_.getCollider(), objects_[i].getCollider()))
				{
					found = true;
					interactableID = i;
				}
			}
			if (found) renderPickupTxt = true;
			else
			{
				renderPickupTxt = false;
				interactableID = -1;
			}

			if (found)
				renderPickupTxt = true;
			else
			{
				renderPickupTxt = false;
				interactableID = -1;
			}
			if (!canShoot)
			{
				if (reload())

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
			}


			clipUsed = 0;
			canShoot = true;
			if (!player_.getAlive())
			{
				gameOver = true;
			}


			handleTrigger();



			if (!player_.getCanTakeDamage())
			{
				if (player_.invincibility())
				{
					player_.setCanTakeDamage(true);
				}
			}
			if (!player_.getCanPunch())
			{
				for (int i(0); i < gconsts::Gameplay::MAXENEMIES; i++)
				{
					if (isCollision(player_.getPunchCollider(), enemies_[i].getCollider()) && enemies_[i].getCanTakeDamage())
					{
						enemies_[i].setCanTakeDamage(false);
						enemies_[i].takeDamage(player_.getPunchDamage());
						if (enemies_[i].getCurrentHealth() <= 0)
						{
							enemies_[i].kill();
						}
					}
				}
			}
			light_.setPosition(player_.getPosition().x - light_.getGlobalBounds().width / 2, player_.getPosition().y - light_.getGlobalBounds().height / 2);
			camera_->update(delta, player_.getPosition(), player_.getSprinting(), player_.getMovementVector());

			//GUI TEXT
			clip = gconsts::Gameplay::MAXBULLETS - bulletIndex;
			if (weaponSelected == PUNCH) ammo_.setString("Weapon : Punch");
			if (weaponSelected == PISTOL) ammo_.setString("Weapon : Pistol	  Ammo : " + to_string(clip) + " / " + to_string(maxAmmo));
			ammo_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(10, 30)));
			reloading_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(10, 50)));
		}
		else
		{

			light_.setPosition(player_.getPosition().x - light_.getGlobalBounds().width / 2, player_.getPosition().y - light_.getGlobalBounds().height / 2);
			camera_->update(delta, player_.getPosition(), player_.getSprinting(), player_.getMovementVector());

		}
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


			if (evnt.key.code == sf::Mouse::Left && weaponSelected == PISTOL)
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
		if (evnt.key.code == sf::Mouse::Left && player_.getCanPunch() && weaponSelected == PUNCH)
		{
			sf::Time frameTime = sf::milliseconds(60);
			player_.setFrameTime(frameTime);
			player_.setAnimationState(1);
			player_.setAnimation(player_.getPlayerPunchAnimation());
			player_.punch();
		}
	}
	if (evnt.type == sf::Event::KeyPressed)
	{
		if (evnt.key.code == sf::Keyboard::R && maxAmmo > 0 && weaponSelected != PUNCH)
		{
			canShoot = false;
		}

		if (evnt.key.code == sf::Keyboard::P && gameOver)
		{
			reset();
		}
		if (evnt.key.code == sf::Keyboard::E && pausedText)
		{
			pausedText = false;
		}
		if (evnt.key.code == sf::Keyboard::E && renderPickupTxt)
		{
			const int funcID = objects_[interactableID].getFuncID();
			switch (funcID)
			{
			case 0://gun sprite
				objects_[interactableID].pickup();
				gunPickedup = true;
				weaponSelected = PISTOL;
				break;
			case 1://health pickup
				player_.pickupHealth(50);
				objects_[interactableID].pickup();
				break;
			case 2:
				objects_[interactableID].switchSprite(0);
				break;
			case 5://big ammo pack
				maxAmmo += 64;
				objects_[interactableID].pickup();
				break;
			case 6://paper note
			case 7://book note
				pausedText = true;
				renderPickupTxt = false;
				textBox_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(100, 75)));
				noteTxt_.setString(objects_[interactableID].getText());
				noteTxt_.setPosition(renderTexture_->mapPixelToCoords(sf::Vector2i(110, 90)));
				objects_[interactableID].pickup();
				break;
			}
		}
		if (evnt.key.code == sf::Keyboard::Num1)
		{
			weaponSelected = PUNCH;
		}
		if (evnt.key.code == sf::Keyboard::Num2 && gunPickedup)
		{
			weaponSelected = PISTOL;
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

	player_.setPosition(playerStart_);
	player_.setAlive(true);
	player_.resetHealth();

	eManage_->reset();


	for (int i(0); i < gconsts::Gameplay::MAXBULLETS; i++)
	{
		bullets_[i].setPosition(0, 0);
		bullets_[i].setRotation(0);
		bullets_[i].setAlive(false);
	}


	for (EnemySpawner& s : spawn_)
	{
		s.reset();
		if (!s.isTriggeredSpawner())
			s.spawnEnemies();
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
		return (true);
	}
	return (false);
}

void PlayState::deinit()
{
	delete camera_;
	delete eManage_;
}

void PlayState::drawLights()
{
	lightRenderTxt_.clear();
	lightRenderTxt_.setView(renderTexture_->getView());
	//for (int i(0); i < lights_.size(); ++i)
	//lightRenderTxt_.draw(lights_[i].shape);
	for (int i(0); i < static_cast<int>(lightList_.size()); ++i)
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

	for (int i(0); i < static_cast<int>(objects_.size()); i++)
	{
		sceneRender_.draw(objects_[i]);
	}
	player_.setOrigin(0.5, 0.5f);
	if (player_.getAlive())
	{
		sceneRender_.draw(player_);
		//sceneRender_.draw(player_.colShape_);
	}

	player_.setOrigin(0.f, 0.f);

	eManage_->draw();


	sceneRender_.display();
}