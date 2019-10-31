#pragma once
#ifndef __Ramp__
#define __Ramp__

#include <vector>

#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"

class Ramp :
	public DisplayObject
{
public:
	Ramp();
	~Ramp();

	// Inherited via GameObject
	void draw() override;

	void update() override;

	void clean() override;


	void move();

	// pathfinding behaviours
	Tile* getTile();
	void setTile(Tile* newTile);

	glm::vec2 getTarget();
	void setTarget(glm::vec2 position);
private:

	void m_checkBounds();

	void m_reset();

	float m_maxSpeed;
	double m_currentDirection;
	float m_turnSpeed;
	float m_steerForce;
	glm::vec2 m_target;

	Tile* m_currentTile;
};


#endif /* defined (__Ramp__) */

