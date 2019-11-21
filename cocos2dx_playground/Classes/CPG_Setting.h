#pragma once

#include "cocos/math/CCGeometry.h"

namespace CPG
{
	class Setting
	{
	private:
		Setting();

	public:
		static void load( cocos2d::Size& frame_resolution, cocos2d::Size& design_resolution );
	};
}