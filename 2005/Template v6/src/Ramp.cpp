#include "Ramp.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "PlayScene.h"
#include "GLM/gtx/string_cast.hpp"
using namespace std;

Ramp::Ramp() :
	m_maxSpeed(5.0f), m_currentDirection(0.0f), m_turnSpeed(2.0f), m_steerForce(0.1f), m_currentTile(NULL)
{
	TheTextureManager::Instance()->load("../Assets/textures/ramp1.png",
		"Ramp", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("Ramp");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(400.0f, 500.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(true);
	setType(GameObjectType::SHIP);
	setState(State::IDLE);

}


Ramp::~Ramp()
{
}

void Ramp::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("Ramp", xComponent, yComponent, TheGame::Instance()->getRenderer(), m_currentDirection, 255, true);
}

void Ramp::clean()
{
	delete m_currentTile;
	m_currentTile = NULL;
}

void Ramp::update()
{
	move();

}

void Ramp::move()
{


}

Tile * Ramp::getTile()
{
	return m_currentTile;
}

void Ramp::setTile(Tile* newTile)
{
	m_currentTile = newTile;
}

glm::vec2 Ramp::getTarget()
{
	return m_target;
}

void Ramp::setTarget(glm::vec2 position)
{
	m_target = position;
}


void Ramp::m_checkBounds()
{

	if (getPosition().x > Config::SCREEN_WIDTH)
	{
		setPosition(glm::vec2(0.0f, getPosition().y));
	}

	if (getPosition().x < 0)
	{
		setPosition(glm::vec2(800.0f, getPosition().y));
	}

	if (getPosition().y > Config::SCREEN_HEIGHT)
	{
		setPosition(glm::vec2(getPosition().x, 0.0f));
	}

	if (getPosition().y < 0)
	{
		setPosition(glm::vec2(getPosition().x, 600.0f));
	}

}

void Ramp::m_reset()
{
	setIsColliding(false);
	int halfWidth = getWidth() * 0.5;
	int xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	int yComponent = -getHeight();
	setPosition(glm::vec2(xComponent, yComponent));
}
