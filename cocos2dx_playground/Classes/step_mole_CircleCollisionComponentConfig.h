#pragma once

#include "cocos/2d/CCNode.h"
#include "cocos/2d/CCComponent.h"
#include "cocos/base/CCRefPtr.h"

namespace step_mole
{
	struct CircleCollisionComponentConfig
	{
		bool use_radius_helper;
		bool use_guide_helper;
		bool use_indicator_helper;
	};
}
