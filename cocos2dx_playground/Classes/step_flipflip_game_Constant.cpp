#include "step_flipflip_game_Constant.h"

#include <cassert>

namespace
{
	const char* CardIndicatorSpriteFrameName = "step_flipflip_card_indicator.png";

	const char* CardBackSideSpriteFrameName = "step_flipflip_card_back_0.png";

	const std::array<char*, static_cast<std::size_t>( step_flipflip::game::eCardType::SIZE )> CardFrontSpriteFrameNames = {
		"step_flipflip_card_front_0.png"
		, "step_flipflip_card_front_1.png"
		, "step_flipflip_card_front_2.png"
		, "step_flipflip_card_front_3.png"
		, "step_flipflip_card_front_4.png"
		, "step_flipflip_card_front_5.png"
	};
}

namespace step_flipflip
{
	namespace game
	{
		const char* GetSpriteFrameName_CardIndicator()
		{
			return CardIndicatorSpriteFrameName;
		}
		const char* GetSpriteFrameName_CardBackSide()
		{
			return CardBackSideSpriteFrameName;
		}
		const char* GetSpriteFrameName_CardFrontSide(const eCardType card_type )
		{
			const auto index = static_cast<std::size_t>( card_type );
			assert( static_cast<std::size_t>( eCardType::FIRST ) <= index && index < static_cast<std::size_t>( eCardType::SIZE ) );

			return CardFrontSpriteFrameNames[index];
		}

		const std::array<char*, static_cast<std::size_t>( step_flipflip::game::eCardType::SIZE )>& GetSpriteFrameNameContainer_CardFrontSide()
		{
			return CardFrontSpriteFrameNames;
		}
	}
}
