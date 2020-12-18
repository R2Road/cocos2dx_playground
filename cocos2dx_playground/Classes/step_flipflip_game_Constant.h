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

		struct StageConfig
		{
			const int Width = 1;
			const int Height = 1;
		};
		const StageConfig STAGE_CONFIG { 5, 4 };

		const char* GetSpriteFrameName_CardIndicator();
		const char* GetSpriteFrameName_CardBackSide();
		const char* GetSpriteFrameName_CardFrontSide( const eCardType card_type );
		const std::array<char*, static_cast<std::size_t>( eCardType::SIZE )>& GetSpriteFrameNameContainer_CardFrontSide();
	}
}
