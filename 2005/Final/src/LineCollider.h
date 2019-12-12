#pragma once
#ifndef __LineCollider__
#define __LineCollider__

// 3rd party includes
#include "GLM/vec2.hpp"

#include "Collider.h"

class LineCollider : public Collider
{
public:
	LineCollider(glm::vec2 start = glm::vec2(0.0f, 0.0f), glm::vec2 end = glm::vec2(0.0f, 0.0f)) : m_start(start), m_end(end) { setCollisionShape(LINE); }
	~LineCollider() {}

	// getters and setters
	glm::vec2 getStart() { return m_start; }
	void setStart(glm::vec2 newStart) { m_start = newStart; }

	glm::vec2 getEnd() { return m_end; }
	void setEnd(glm::vec2 newEnd) { m_end = newEnd; }
	
private:
	glm::vec2 m_start;
	glm::vec2 m_end;
};

#endif /* defined (__LineCollider__) */