#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Ball.h"

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
	/*Label* m_pStartLabel;
	Label* m_pInstructionsLabel;*/

	Ship* m_pShip;
	Ball* m_pBall;




	glm::vec2 m_mousePosition;

	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateUI();

	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	// Physics Variables
	float m_gravity = 9.8f;
	int m_PPM = 10; // pixels per meter
	glm::vec2 m_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f);
	
	// Physics functions
	void m_move();
	bool AABB(GameObject &obj1, GameObject &obj2);
};

#endif /* defined (__START_SCENE__) */