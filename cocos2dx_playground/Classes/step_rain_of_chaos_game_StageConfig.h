#pragma once

#include "math/CCGeometry.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageConfig
		{
		public:
			cocos2d::Vec2 mCenter;
			cocos2d::Rect mBulletLifeArea;
			cocos2d::Rect mBulletGenerateArea;
		};
	}
}
