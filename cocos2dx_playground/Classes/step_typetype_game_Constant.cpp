#include "step_typetype_game_Constant.h"

namespace step_typetype
{
	namespace game
	{
		cocos2d::Size CalculateStageSize( const std::size_t stage_length )
		{
			const cocos2d::Size stage_size(
				GameConfig.MarginSize.width
				+ ( stage_length * GameConfig.LetterSize.width )
				+ GameConfig.MarginSize.width

				, GameConfig.MarginSize.height
				+ GameConfig.LetterSize.height
				+ GameConfig.MarginSize.height
			);

			return stage_size;
		}
	}
}
