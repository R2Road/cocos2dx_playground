#pragma once

#include "base/ccTypes.h"
#include "math/CCGeometry.h"

namespace step_defender
{
	namespace game
	{
		const struct
		{
			cocos2d::Size WorldSize = cocos2d::Size( 960.f, 320.f );
			cocos2d::Color4B SkyColor = cocos2d::Color4B::BLACK;
			cocos2d::Size BottomSize = cocos2d::Size( 960.f, 80.f );
		} WorldConfig;
	}
}
