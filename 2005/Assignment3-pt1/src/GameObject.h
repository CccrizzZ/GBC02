#pragma once
#ifndef __GameObject__
#define __GameObject__

#include <glm\vec2.hpp>
#include <string>

enum GameObjectType {
	NONE = -1,
	SHIP,
	TARGET,
	PLANET,
	MINE,
	TILE,
	NUM_OF_TYPES
};

enum State {
	NO_STATE= -1,
	IDLE,
	SEEK,
	ARRIVE,
	AVOID,
	FLEE,
	NUM_OF_STATES
};

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	// Draw the object
	virtual void draw() = 0;

	// Update the object
	virtual void update() = 0;

	// remove anything that needs to be deleted
	virtual void clean() = 0;

	// getters for common variables
	glm::vec2 getPosition();
	glm::vec2 getRotation();
	glm::vec2 getScale();
	glm::vec2 getVelocity();
	glm::vec2 getAcceleration();
	int getWidth();
	int getHeight();
	bool getIsColliding();
	GameObjectType getType();
	State getState();

	// setters for common variables
	void setPosition(glm::vec2 newPosition);
	void setWidth(int newWidth);
	void setHeight(int newHeight);
	void setVelocity(glm::vec2 newVelocity);
	void setIsColliding(bool collision);
	void setType(GameObjectType newType);
	void setState(State newState);
	void setAcceleration(glm::vec2 newAcceleration);
	void reboundX()
	{
		float xTemp = -(getVelocity().x);
		float yTemp = getVelocity().y;

		setVelocity(glm::vec2(xTemp, yTemp));
	}

	void reboundY()
	{
		float xTemp = getVelocity().x;
		float yTemp = -(getVelocity().y);

		setVelocity(glm::vec2(xTemp, yTemp));
	}

private:
	// transform variables
	glm::vec2 m_position;
	glm::vec2 m_rotation;
	glm::vec2 m_scale;

	// movement variables
	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;

	// size variables
	int m_width;
	int m_height;

	// animation variables
	int m_currentRow;
	int m_currentFrame;
	int m_numFrames;
	std::string m_textureID;

	// collision variables
	bool m_isColliding;
	GameObjectType m_type;
	State m_state;
};

#endif