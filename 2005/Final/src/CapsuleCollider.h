#pragma once
#ifndef __CapsuleCollider__
#define __CapsuleCollider__

// 3rd party includes
#include "GLM/vec2.hpp"

#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(glm::vec2 start = glm::vec2(0.0f, 0.0f), glm::vec2 end = glm::vec2(0.0f, 0.0f), int radius = 0):m_start(start), m_end(end), m_radius(radius){setCollisionShape(CAPSULE); }
	~CapsuleCollider() {}

	// getters and setters
	glm::vec2 getStart() { return m_start; }
	void setStart(glm::vec2 newStart) { m_start = newStart; }

	glm::vec2 getEnd() { return m_end; }
	void setEnd(glm::vec2 newEnd) { m_end = newEnd; }

	int getRadius() { return m_radius; }
	void setRadius(int newRadius) { m_radius = newRadius; }
private:
	glm::vec2 m_start;
	glm::vec2 m_end;
	int m_radius;
};


#endif /* defined (__CapsuleCollider__) */