#pragma once
#ifndef __Collider__
#define __Collider__

enum CollisionShape
{
	NO_COLLIDER,
	CIRCLE,
	LINE,
	CAPSULE,
	RECTANGLE
};

class Collider
{
public:
	Collider() {}
	virtual ~Collider() {}

	CollisionShape getCollisionShape() { return m_collisionShape; }
	void setCollisionShape(CollisionShape newShape) { m_collisionShape = newShape; }
	
protected:
	CollisionShape m_collisionShape;
};

#endif /* defined (__Collider__) */