#pragma once

#include "math/CCGeometry.h"

namespace step_typetype
{
	namespace game
	{
		const struct
		{
			const int StageMaxLength = 20;

			const cocos2d::Size LetterSize = cocos2d::Size( 14.f, 14.f );
			const cocos2d::Size MarginSize = cocos2d::Size( 2.f, 2.f );
		} GameConfig;

		cocos2d::Size CalculateStageSize( const std::size_t stage_length );
	}
}
