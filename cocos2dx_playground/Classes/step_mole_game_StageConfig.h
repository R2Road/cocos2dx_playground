#pragma once

#include "math/CCGeometry.h"

namespace step_mole
{
	namespace game
	{
		struct StageConfig
		{
			const int BlockCount_Horizontal = 8;
			const int BlockCount_Vercital = 6;
			const cocos2d::Size BlockSize = cocos2d::Size( 30.f, 30.f );
		};
	}
}
