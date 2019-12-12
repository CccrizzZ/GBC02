#pragma once
#ifndef __CircleCollider__
#define __CircleCollider__

// 3rd party includes
#include "GLM/vec2.hpp"

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(glm::vec2 centre = glm::vec2(0.0f, 0.0f), int radius = 0) : m_centre(centre), m_radius(radius) { setCollisionShape(CIRCLE); }
	~CircleCollider(){}

	// getters and setters
	glm::vec2 getCentre() { return m_centre; }
	void setCentre(glm::vec2 newCentre) { m_centre = newCentre; }

	int getRadius() { return m_radius; }
	void setRadius(int newRadius) { m_radius = newRadius; }
	
private:
	glm::vec2 m_centre;
	int m_radius;
};

#endif /* defined (__CircleCollider__) */