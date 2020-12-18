#pragma once

#include <array>

#include "math/CCGeometry.h"

namespace step_flipflip
{
	namespace game
	{
		enum class eCardType
		{
			A
			, B
			, C
			, D
			, E

			, SIZE
		};

		struct StageConfig
		{
			const int Width = 1;
			const int Height = 1;
			const cocos2d::Size CardAreaSize = cocos2d::Size( 10.f, 10.f );
		};
		const StageConfig STAGE_CONFIG { 5, 4, cocos2d::Size( 40.f, 54.f ) };

		const char* GetSpriteFrameName_CardIndicator();
		const char* GetSpriteFrameName_CardBackSide();
		const char* GetSpriteFrameName_CardFrontSide( const eCardType card_type );
		const std::array<char*, static_cast<std::size_t>( eCardType::SIZE )>& GetSpriteFrameNameContainer_CardFrontSide();
	}
}
