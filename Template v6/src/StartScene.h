#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "LootCrate.h"
#include "Ramp.h"
#include "ship.h"


class StartScene : public Scene
{
public:
	StartScene();
	~StartScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:

	Ship* m_pShip;
	LootCrate* m_pCrate; 
	Ramp* m_pRamp;
};

#endif /* defined (__START_SCENE__) */