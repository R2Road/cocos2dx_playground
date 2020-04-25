#include "step_clickclick_game_Block.h"

#include <algorithm>

namespace step_clickclick
{
	namespace game
	{
		Block::Block( const int index ) :
			mIndex( index )
			, mBlockType( eBlockType::Different )
			, mActive( false )
			, mLife( 0 )
		{}

		void Block::Reset( eBlockType type, const int life )
		{
			mBlockType = type;
			mActive = true;
			mLife = life;
		}
		void Block::DecreaseAction()
		{
			mLife = std::max( 0, mLife - 1 );
			mActive = ( 0 < mLife );
		}
		void Block::IncreaseAction()
		{
			mLife = std::min( 100, mLife + 1 );
			mActive = ( 0 < mLife );
		}
		void Block::DieAction()
		{
			mLife = 0;
			mActive = false;
		}
	} // namespace game
} // namespace step_clickclick
