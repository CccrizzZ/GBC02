#pragma once
#ifndef __Platform__
#define __Platform__

#include "DisplayObject.h"

class Platform : public DisplayObject
{
public:
	Platform();
	~Platform();
	
	// Inherited via GameObject
	virtual void draw() override;

	virtual void update() override;

	virtual void clean() override;
private:
};

#endif /* defined (__Platform__) */