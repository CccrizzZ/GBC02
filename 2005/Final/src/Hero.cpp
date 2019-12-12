#include "Hero.h"
#include "Game.h"

Hero::Hero()
{
	TheTextureManager::Instance()->load("../Assets/textures/hero/idle-1.png",
		"hero", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("hero");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(0.0f, 0.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setType(GameObjectType::HERO);
}

Hero::~Hero()
{

}

void Hero::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("hero", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0, 255, true);

}

void Hero::update()
{
	
}

void Hero::clean()
{
}
