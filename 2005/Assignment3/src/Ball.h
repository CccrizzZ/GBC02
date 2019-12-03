#pragma once
#ifndef __Ball__
#define __Ball__

#include <vector>

#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"

class Ball :
	public DisplayObject
{
public:
	Ball();
	~Ball();

	// Inherited via GameObject
	void draw() override;

	void update() override;

	void clean() override;

	void move();

	void m_checkBounds();

	void reboundX();
	
	void reboundY();

private:
	float m_maxSpeed;
	double m_currentDirection;
	float m_turnSpeed;
	float m_steerForce;

	Tile *m_currentTile;
};

#endif 
