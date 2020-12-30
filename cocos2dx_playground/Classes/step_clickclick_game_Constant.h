#pragma once

#include "math/CCGeometry.h"

namespace step_clickclick
{
	namespace game
	{
		const cocos2d::Size BlockSize( 32.f, 32.f );
		const cocos2d::Size BlockMarginSize( 2.f, 2.f );

		enum class eBlockType
		{
			Single,
			Same,
			Different,

			SIZE,
			FIRST = Single,
			END = Different,
		};

		const char* ConvertBlockType2SpriteFrameName( const eBlockType block_type );


		enum eEffectIndex
		{
			Increase
			, Decrease
			, Die
		};
	}
}
