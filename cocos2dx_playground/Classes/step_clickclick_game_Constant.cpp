#include "step_clickclick_game_Constant.h"

#include <array>
#include <cassert>

namespace
{
	const std::array<char*, static_cast<std::size_t>( step_clickclick::game::eBlockType::SIZE )> SpriteFrameNames = {
		"step_clickclick_block_single.png"
		, "step_clickclick_block_together.png"
		, "step_clickclick_block_different.png"
	};
}

namespace step_clickclick
{
	namespace game
	{
		const char* ConvertBlockType2SpriteFrameName( const eBlockType block_type )
		{
			const auto index = static_cast<std::size_t>( block_type );
			assert( 0 <= index && index < static_cast<std::size_t>( eBlockType::SIZE ) );

			return SpriteFrameNames[index];
		}
	}
}
