#pragma once
#ifndef __Hero__
#define __Hero__

#include "DisplayObject.h"

class Hero : public DisplayObject
{
public:
	Hero();
	~Hero();
	
	// Inherited via GameObject
	virtual void draw() override;

	virtual void update() override;

	virtual void clean() override;
private:
};

#endif /* defined (__Hero__) */