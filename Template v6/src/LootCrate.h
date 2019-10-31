#pragma once
#ifndef __LootCrate__
#define __LootCrate__

#include <vector>
#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"

class LootCrate :
	public DisplayObject
{
public:
	LootCrate(){};
	~LootCrate(){};

	// Inherited via GameObject
	void draw() override{};

	void update() override{};

	void clean() override{};

	void turnRight();
	void turnLeft();
	void move();

	// pathfinding behaviours
	Tile* getTile();
	void setTile(Tile* newTile);

	glm::vec2 getTarget();
	void setTarget(glm::vec2 position);
private:


	void m_checkState();
	void m_checkBounds();

	void m_reset();

	// steering behaviours
	void m_seek();
	float m_maxSpeed;
	double m_currentDirection;
	float m_turnSpeed;
	float m_steerForce;
	glm::vec2 m_target;

	Tile* m_currentTile;
};


#endif /* defined (__LootCrate__) */

