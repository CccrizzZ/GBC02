#include "StartScene.h"
#include "Game.h"
#include <ctime>
#include "GLM/gtx/string_cast.hpp"
#include <algorithm>
#include "TileComparators.h"
#include <iomanip>
#include <iostream>
#include "CollisionManager.h"
using namespace std;

StartScene::StartScene()
{

	StartScene::start();
}

StartScene::~StartScene()
{
}

void StartScene::start()
{

	m_pRamp = new Ramp();
	addChild(m_pRamp);

	m_pShip = new Ship();
	addChild(m_pShip);

}


void StartScene::draw()
{

	m_pRamp->draw(); 
	m_pShip->draw();


}

void StartScene::update()
{
	m_pRamp->update();
	m_pShip->update();

}

void StartScene::clean()
{
	cout << "cleaning start scene" <<endl;
	delete m_pRamp;
	delete m_pShip;

	removeAllChildren();

}

void StartScene::handleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_1:
				TheGame::Instance()->changeSceneState(SceneState::PLAY_SCENE);
				break;
			case SDLK_2:
				TheGame::Instance()->changeSceneState(SceneState::END_SCENE);
				break;
			}
			break;

		default:
			break;
		}
	}
}


