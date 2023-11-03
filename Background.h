#pragma once

#include "GameObject.h"


class Background : public GameObject
{
public:
	Background()
		: GameObject{ CollisionID::Level, CollisionID::None }
	{
		isInView_ = true;
	}


	void SetIsInView(bool isInView) override
	{
		// Always in view.
		isInView = true;
	}
};

