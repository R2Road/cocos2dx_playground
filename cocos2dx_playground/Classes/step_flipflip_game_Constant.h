#pragma once

#include <array>

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

		const char* GetSpriteFrameName_CardIndicator();
		const char* GetSpriteFrameName_CardBackSide();
		const char* GetSpriteFrameName_CardFrontSide( const eCardType card_type );
		const std::array<char*, static_cast<std::size_t>( eCardType::SIZE )>& GetSpriteFrameNameContainer_CardFrontSide();
	}
}
