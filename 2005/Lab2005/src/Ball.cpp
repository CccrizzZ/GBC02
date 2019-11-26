#include "Ball.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "PlayScene.h"
#include "GLM/gtx/string_cast.hpp"

Ball::Ball() : 
    m_maxSpeed(5.0f), m_currentDirection(0.0f), m_turnSpeed(2.0f), m_steerForce(0.1f), m_currentTile(NULL)
{
    TheTextureManager::Instance()->load(
        "../Assets/textures/planet.png",
        "planet", 
        TheGame::Instance()->getRenderer()
    );

    glm::vec2 size = TheTextureManager::Instance()->getTextureSize("planet");
    setWidth(size.x);
    setHeight(size.y);
    setPosition(glm::vec2(0.0f, 0.0f));
    setVelocity(glm::vec2(0.0f, 0.0f));
    setAcceleration(glm::vec2(0.0f, 0.0f));
    setIsColliding(false);
    setType(GameObjectType::PLANET);
    setState(State::IDLE);
}

Ball::~Ball()
{
}

void Ball::draw()
{
    int xComponent = getPosition().x;
    int yComponent = getPosition().y;

    TheTextureManager::Instance()->draw(
        "planet",
        xComponent,
        yComponent,
        TheGame::Instance()->getRenderer(),
        m_currentDirection,
        255,
        true);
}

void Ball::update()
{
    move();
    m_checkBounds();
}

void Ball::clean()
{
    delete m_currentTile;
    m_currentTile = NULL;
}

void Ball::move()
{
    glm::vec2 newPos = getPosition() + getVelocity();
    setPosition(newPos);
    
    if (getVelocity().x != 0)
    {
        if (getVelocity().x > 0)
        {
            setVelocity(glm::vec2(getVelocity().x - 0.01f , getVelocity().y));
        }
        else if(getVelocity().x < 0)
        {
            setVelocity(glm::vec2(getVelocity().x + 0.01f, getVelocity().y));
        }
    }

    if (getVelocity().y != 0)
    {
        if (getVelocity().y > 0)
        {
            setVelocity(glm::vec2(getVelocity().x, getVelocity().y - 0.01f));
        }
        else if (getVelocity().y < 0)
        {
            setVelocity(glm::vec2(getVelocity().x, getVelocity().y + 0.01f)); 
        }
    }
}

void Ball::m_checkBounds()
{
    if (getPosition().x > 800 - getWidth()/2)
    {
        setPosition(glm::vec2(800.0f - getWidth()/2, getPosition().y));
        reboundX();
    }

    if (getPosition().x < 0 + getWidth() / 2)
    {
        setPosition(glm::vec2(0.0f + getWidth()/2, getPosition().y));
        reboundX();
    }

    if (getPosition().y > 600.0f - getHeight() / 2)
    {
        setPosition(glm::vec2(getPosition().x, 600.0f - getHeight()/2));
        reboundY();
    }

    if (getPosition().y < 0 + getHeight() / 2)
    {
        setPosition(glm::vec2(getPosition().x, 0.0f + getHeight()/2));
        reboundY();
    }
}

void Ball::reboundX()
{
    float xTemp = -(getVelocity().x);
    float yTemp = getVelocity().y;

    setVelocity(glm::vec2(xTemp,yTemp));
}

void Ball::reboundY()
{
    float xTemp = getVelocity().x;
    float yTemp = -(getVelocity().y);

    setVelocity(glm::vec2(xTemp, yTemp));
}
