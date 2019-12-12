#include "Target.h"
#include "Game.h"
#include "Util.h"

Target::Target()
{
	TheTextureManager::Instance()->load("../Assets/textures/Circle.png",
		"circle", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(100.0f, 100.0f));
	setVelocity(glm::vec2(0, 0));
	setIsColliding(false);
	setType(GameObjectType::TARGET);
}

Target::~Target()
{
}

void Target::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;
	TheTextureManager::Instance()->draw("circle", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	glm::vec2 newPosition = getPosition() + getVelocity() * 2.0f;
	setPosition(newPosition);
}

void Target::m_checkBounds()
{
	if((getPosition().y > 600.0f + getHeight() * 0.5f) || // bottom 
	   (getPosition().y < -getHeight() * 0.5f) || // top
	   (getPosition().x > 800.0f + getWidth() * 0.5) || // right
	   (getPosition().x < -getWidth() * 0.5f)) // left
	{
		reset();
	}
}

void Target::reset()
{
	int d4 = ceil(Util::RandomRange(0.0, 4.0));

	switch(d4)
	{
	case 1: // top
		setPosition(glm::vec2(Util::RandomRange(getWidth() * 0.5f, 800.0f - getWidth()), -getHeight() * 0.5f));
		setVelocity(glm::vec2(Util::RandomRange(-1.0f, 1.0f), 1.0f));
		break;
	case 2: // right
		setPosition(glm::vec2(800.0f +getWidth() * 0.5f, Util::RandomRange(getHeight() * 0.5f, 600.0f - getHeight())));
		setVelocity(glm::vec2(-1.0f, Util::RandomRange(-1.0f, 1.0f)));
		break;
	case 3: // bottom
		setPosition(glm::vec2(Util::RandomRange(getWidth() * 0.5f, 800.0f - getWidth()), 600.0f + getHeight() * 0.5f));
		setVelocity(glm::vec2(Util::RandomRange(-1.0f, 1.0f), -1.0f));
		break;
	case 4: // left
		setPosition(glm::vec2(-getWidth() * 0.5f, Util::RandomRange(getHeight() * 0.5f, 600.0f - getHeight())));
		setVelocity(glm::vec2(1.0f, Util::RandomRange(-1.0f, 1.0f)));
		break;
	}
	
}
