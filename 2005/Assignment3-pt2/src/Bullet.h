#pragma once
#ifndef __Bullet__
#define __Bullet__

#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

class Bullet : public DisplayObject
{
public:
	Bullet();
	~Bullet();

	// Inherited via GameObject
	virtual void draw() override;

	virtual void update() override;

	virtual void clean() override;

	// pathfinding behaviours
	Tile* getTile();
	void setTile(Tile* newTile);

	void ResetBullet()
	{
		int randposX = rand() % 600 + (-600);
		int randposY = rand() % 600 + 1;
		setPosition(glm::vec2(randposX, randposY));
	}


private:
	Tile* m_currentTile;
};


#endif /* defined (__Bullet__) */