#include "Bullet.h"
#include "Game.h"

Bullet::Bullet() :
	m_currentTile(NULL)
{
	TheTextureManager::Instance()->load("../Assets/textures/Bullet.png",
		"Bullet", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("Bullet");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(0.0f, 0.0f));
	setVelocity(glm::vec2(10.0f, 0.0f));
	setType(GameObjectType::MINE);
}

Bullet::~Bullet()
{
}

void Bullet::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("Bullet", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0, 255, true);
}

void Bullet::update()
{

	if (getPosition().x >= 820)
	{
		ResetBullet();
	}
	

	glm::vec2 newPosition =  getPosition() + getVelocity();
	setPosition(newPosition);
}

void Bullet::clean()
{
}

Tile * Bullet::getTile()
{
	return m_currentTile;
}

void Bullet::setTile(Tile * newTile)
{
	m_currentTile = newTile;
}
