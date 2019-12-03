#include "StartScene.h"
#include "Game.h"
#include <ctime>
#include "GLM/gtx/string_cast.hpp"
#include <algorithm>
#include "TileComparators.h"
#include <iomanip>
#include <iostream>
using namespace std;

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
{
}

bool StartScene::AABB(GameObject& obj1, GameObject& obj2)
{
	int tempW1 = obj1.getWidth() / 2;
	int tempW2 = obj2.getWidth() / 2;
	int tempH1 = obj1.getHeight() / 2;
	int tempH2 = obj2.getHeight() / 2;
	float obj1_X = obj1.getPosition().x;
	float obj2_X = obj2.getPosition().x;
	float obj1_Y = obj1.getPosition().y;
	float obj2_Y = obj2.getPosition().y;


	if
	((
		obj1_X + tempW1 >= obj2_X - tempW2 &&
		obj1_X - tempW1 <= obj2_X + tempW2) &&
		(obj1_Y + tempH1 >= obj2_Y - tempH2 &&
		obj1_Y - tempH1 <= obj2_Y + tempH2
	))
	{
		if ((
			obj1_X + tempW1 >= obj2_X - tempW2 && obj1_Y + tempH1 >= obj2_Y - tempH2) || 
			(obj1_X + tempW1 >= obj2_X - tempW2 && obj1_Y - tempH1 <= obj2_Y + tempH2) || 
			(obj1_X - tempW1 <= obj2_X + tempW2 && obj1_Y + tempH1 >= obj2_Y - tempH2) || 
			(obj1_X + tempW1 >= obj2_X - tempW2) || 
			(obj1_X - tempW1 <= obj2_X + tempW2
		))
		{
			if (obj1.getVelocity().x != 0 && obj1.getVelocity().y !=0)
			{
				obj2.setVelocity(glm::vec2(obj1.getVelocity().x, obj1.getVelocity().y));
			}
			return true;
		}
	}
	return false;
}

void StartScene::draw()
{


	m_pShip->draw();
	m_pBall->draw();

	// ImGui Rendering section - DO NOT MOVE OR DELETE
	if (m_displayUI)
	{
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
		SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 255, 255, 255, 255);
	}
}

void StartScene::update()
{
	m_move();
	m_pBall->update();
	m_pShip->update();
	
	if(AABB(*m_pShip,*m_pBall))
	{	
		if (m_pShip->getVelocity().x == 0)
		{
			m_pBall->reboundX();
		}
		
		if (m_pShip->getVelocity().y == 0)
		{
			m_pBall->reboundY();			
		}

		m_pBall->setPosition(glm::vec2(
			m_pBall->getPosition().x + m_pShip -> getVelocity().x,
			m_pBall->getPosition().y + m_pShip -> getVelocity().y)
		);
	}

	if (m_displayUI)
	{
		m_updateUI();
	}
}

void StartScene::clean()
{
	delete m_pShip;
	delete m_pBall;

	removeAllChildren();
}

void StartScene::handleEvents()
{
	ImGuiIO& io = ImGui::GetIO();
	int wheel = 0;

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_BACKQUOTE:
				m_displayUI = (m_displayUI) ? false : true;
				break;
			
			/***************************************************************/
			case SDLK_w:
				m_velocity = glm::vec2(0.0f, -2.0f);
				break;
			case SDLK_s:
				m_velocity = glm::vec2(0.0f, 2.0f);
				break;
			case SDLK_a:
				m_velocity = glm::vec2(-2.0f, 0.0f);
				break;
			case SDLK_d:
				m_velocity = glm::vec2(2.0f, 0.0f);
				break;
			}
			{
				int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				m_velocity = glm::vec2(0.0f, 0.0f);
				break;
			case SDLK_s:
				m_velocity = glm::vec2(0.0f, 0.0f);
				break;
			case SDLK_a:
				m_velocity = glm::vec2(0.0f, 0.0f);
				break;
			case SDLK_d:
				m_velocity = glm::vec2(0.0f, 0.0f);
				break;
			}
			{
				int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		default:
			break;
		}
	}

	io.DeltaTime = 1.0f / 60.0f;
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	m_ImGuiKeyMap();
	m_ImGuiSetStyle();
}

void StartScene::start()
{
	m_position = glm::vec2(50.0f, 50.0f);
	m_pShip = new Ship();
	m_pShip->setPosition(m_position);
	addChild(m_pShip);

	m_pBall = new Ball();
	m_pBall -> setPosition(glm::vec2(400.0f, 400.0f));
	addChild(m_pBall);




}

void StartScene::m_ImGuiKeyMap()
{
	ImGuiIO& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
}

void StartScene::m_ImGuiSetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 0.8f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void StartScene::m_updateUI()
{
	// Prepare Window Frame
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // use for debug purposes

	std::string windowString = "Physics Settings ";

	ImGui::Begin(&windowString[0], NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);

	// set window to top left corner
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	/*************************************************************************************************/
	/* MENU                                                                                          */
	/*************************************************************************************************/

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Exit", NULL, &m_exitApp);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Separator();
			ImGui::MenuItem("About", NULL, &m_displayAbout);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (m_exitApp)
	{
		TheGame::Instance()->quit();
	}

	if (m_displayAbout)
	{
		ImGui::Begin("About Physics...", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::Text("Authors:");
		ImGui::Text("Tom Tsiliopoulos ");
		ImGui::End();
	}

	/*************************************************************************************************/
	if (ImGui::Button("Enable Gravity"))
	{
		// m_respawnShips();
		// m_moveAlongPath = false;
	}

	//ImGui::SameLine();

	//if (ImGui::Button("Respawn Planet"))
	//{
	//	/*m_respawnPlanet();
	//	m_moveAlongPath = false;*/
	//}

	//ImGui::SameLine();

	//if (ImGui::Button("Respawn Mines"))
	//{
	//	/*m_respawnMines();
	//	m_moveAlongPath = false;*/
	//}

	//ImGui::SameLine();

	//if (ImGui::Button("Toggle Grid"))
	//{
	//	/*m_debugMode = (m_debugMode) ? false : true;*/
	//}

	//ImGui::SameLine();

	//if (ImGui::Button("Reset All"))
	//{
	//	/*m_resetAll();*/
	//}

	//ImGui::PushItemWidth(80);
	///*if (ImGui::SliderFloat("Manhattan Factor", m_grid[0][0]->getManhanttanFactor(), 0.1f, 10.0f, "%.1f"))
	//{
	//	float newFactor = *m_grid[0][0]->getManhanttanFactor();

	//	for (size_t row = 0; row < m_rowSize; row++)
	//	{
	//		for (size_t col = 0; col < m_colSize; col++)
	//		{
	//			m_grid[col][row]->setManhanttanFactor(newFactor);
	//		}
	//	}
	//}*/

	//ImGui::SameLine();

	///*if (ImGui::SliderFloat("Euclidean Factor", m_grid[0][0]->getEuclideanFactor(), 0.1f, 10.0f, "%.1f"))
	//{
	//	float newFactor = *m_grid[0][0]->getEuclideanFactor();

	//	for (size_t row = 0; row < m_rowSize; row++)
	//	{
	//		for (size_t col = 0; col < m_colSize; col++)
	//		{
	//			m_grid[col][row]->setEuclideanFactor(newFactor);
	//		}
	//	}
	//}*/

	//ImGui::SameLine();

	///*if (ImGui::SliderFloat("Mine Factor", m_grid[0][0]->getMineFactor(), 0.1f, 10.0f, "%.1f"))
	//{
	//	float newFactor = *m_grid[0][0]->getMineFactor();

	//	for (size_t row = 0; row < m_rowSize; row++)
	//	{
	//		for (size_t col = 0; col < m_colSize; col++)
	//		{
	//			m_grid[col][row]->setMineFactor(newFactor);
	//		}
	//	}
	//}*/
	//ImGui::PopItemWidth();

	//if (ImGui::CollapsingHeader("Ship Locations"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;

	//	std::string shipText;
	//	shipText = "Ship " + count;
	//	shipText += " Position: ";
	//	ImGui::Text(shipText.c_str());
	//	ImGui::SameLine();
	//	glm::vec2 pos = m_ship.getPosition();
	//	ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//	count++;
	//	ImGui::PopItemWidth();*/
	//}

	//if (ImGui::CollapsingHeader("Planet Location"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	std::string planetText;
	//	planetText = "Planet Position: ";
	//	ImGui::Text(planetText.c_str());
	//	ImGui::SameLine();
	//	glm::vec2 pos = m_planet.getPosition();
	//	ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//	ImGui::PopItemWidth();*/
	//}

	//if (ImGui::CollapsingHeader("Mine Locations"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;
	//	for (Mine mine : m_pMines)
	//	{
	//		std::string mineText;
	//		mineText = "Mine " + count;
	//		mineText += " Position: ";
	//		ImGui::Text(mineText.c_str());
	//		ImGui::SameLine();
	//		glm::vec2 pos = mine.getPosition();
	//		ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//		count++;
	//	}
	//	ImGui::PopItemWidth();*/
	//}

	//ImGui::Separator();

	//ImGui::PushItemWidth(80);
	////glm::vec2 targetPosition = getTargetPosition();
	///*if (ImGui::SliderFloat("Target X Position", &targetPosition.x, 0.0f, 800.0f, "%.0f"))
	//{
	//	setTargetPosition(targetPosition);
	//}*/
	//ImGui::SameLine();
	/*if (ImGui::SliderFloat("Target Y Position", &targetPosition.y, 0.0f, 600.0f, "%.0f"))
	{
		setTargetPosition(targetPosition);
	}*/
	//ImGui::PopItemWidth();

	//ImGui::Separator();

	//if (ImGui::Button("Find Path"))
	//{
	//	//std::cout << "**** NEW PATH ****" << std::endl;

	//	/*findShortestPath();
	//	m_moveAlongPath = false;*/
	//}

	//if (ImGui::CollapsingHeader("Open Tiles"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;
	//	for (Tile* tile : m_pOpen)
	//	{
	//		std::string tileText;
	//		tileText = "Tile ";
	//		tileText += std::to_string(count);
	//		tileText += " Position: ";
	//		ImGui::Text(tileText.c_str());
	//		ImGui::SameLine();
	//		glm::vec2 pos = tile->getPosition();
	//		ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//		count++;
	//	}
	//	ImGui::PopItemWidth();*/
	//}

	//if (ImGui::CollapsingHeader("Closed Tiles"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;
	//	for (Tile* tile : m_pClosed)
	//	{
	//		std::string tileText;
	//		tileText = "Tile ";
	//		tileText += std::to_string(count);
	//		tileText += " Position: ";
	//		ImGui::Text(tileText.c_str());
	//		ImGui::SameLine();
	//		glm::vec2 pos = tile->getPosition();
	//		ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//		count++;
	//	}
	//	ImGui::PopItemWidth();*/
	//}

	/*if (!m_pOpen.empty())
	{
		if (ImGui::Button("Move Ship Along Path"))
		{
			m_moveAlongPath = true;
			m_pathLength = 0;
		}
	}*/

	// Main Window End
	ImGui::End();
}

void StartScene::m_move()
{
	// m_acceleration = glm::vec2(0.0f, 0.5 * m_gravity * m_PPM);

	// If ship is not at mouse position move ship to mouse position
	if (m_mousePosition.x != m_pShip->getPosition().x && m_mousePosition.y != m_pShip->getPosition().y)
	{
		m_velocity = glm::vec2(roundf((m_mousePosition.x - m_pShip->getPosition().x) / 5), roundf((m_mousePosition.y - m_pShip->getPosition().y) / 5));		
	}
	else
	{
		m_pShip->setPosition(glm::vec2(m_mousePosition.x, m_mousePosition.y));
	}

	std::cout << m_velocity.x << "|" << m_velocity.y << std::endl;

	m_pShip->setVelocity(m_velocity);

	// Set Ship Velocity
	m_position = m_pShip->getPosition() + m_velocity;
	m_pShip->setPosition(m_position);
}
