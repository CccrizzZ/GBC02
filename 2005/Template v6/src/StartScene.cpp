#include "StartScene.h"
#include "Game.h"
#include <ctime>
#include "GLM/gtx/string_cast.hpp"
#include <algorithm>
#include "TileComparators.h"
#include <iomanip>
#include "ship.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
{
}






void StartScene::draw()
{
	// m_pStartLabel->draw();
	// m_pInstructionsLabel->draw();
	m_pShip -> draw();

}






void StartScene::update()
{
	m_pShip -> update();

}






void StartScene::clean()
{
	delete m_pStartLabel;
	delete m_pInstructionsLabel;
	removeAllChildren();
	m_pShip -> clean();
	delete m_pShip;
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
			//case SDLK_1:
			//	TheGame::Instance()->changeSceneState(SceneState::PLAY_SCENE);
			//	break;
			//case SDLK_2:
			//	TheGame::Instance()->changeSceneState(SceneState::END_SCENE);
			//	break;
			}
			break;

		default:
			break;
		}
	}
}

void StartScene::start()
{
	m_pShip = new Ship();
	m_pShip -> setMaxSpeed(0.0);
	// SDL_Color blue = { 0, 0, 255, 255 };
	// m_pStartLabel = new Label("START SCENE", "Dock51", 80, blue, glm::vec2(400.0f, 40.0f));
	// m_pStartLabel->setParent(this);
	// addChild(m_pStartLabel);
	//
	// m_pInstructionsLabel = new Label("Press 1 to Play", "Dock51", 40, blue, glm::vec2(400.0f, 120.0f));
	// m_pInstructionsLabel->setParent(this);
	// addChild(m_pInstructionsLabel);
}
