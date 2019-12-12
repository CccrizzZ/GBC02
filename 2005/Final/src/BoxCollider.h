#pragma once
#ifndef __BoxCollider__
#define __BoxCollider__

// 3rd party includes
#include "GLM/vec2.hpp"

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(glm::vec2 start = glm::vec2(0.0f, 0.0f), int width = 0, int height = 0) :m_start(start), m_width(width), m_height(height) { setCollisionShape(RECTANGLE); }
	~BoxCollider(){}

	// getters and setters
	glm::vec2 getStart() { return m_start; }
	void setStart(glm::vec2 newStart) { m_start = newStart; }

	int getWidth() { return m_width; }
	void setWidth(int newWidth) { m_width = newWidth; }

	int getHeight() { return m_height; }
	void setHeight(int newHeight) { m_height = newHeight; }
	
private:
	glm::vec2 m_start;
	int m_width;
	int m_height;
};


#endif /* defined (__BoxCollider__) */