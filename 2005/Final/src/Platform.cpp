#include "Platform.h"
#include "Game.h"

Platform::Platform()
{
	TheTextureManager::Instance()->load("../Assets/textures/platform.png",
		"platform", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("platform");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(0.0f, 0.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setType(GameObjectType::PLATFORM);
}

Platform::~Platform()
{
}

void Platform::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("platform", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0, 255, true);
}

void Platform::update()
{
}

void Platform::clean()
{
}
