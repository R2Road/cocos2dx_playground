#include "step_typetype_game_Stage.h"

#include <algorithm>
#include <cassert>
#include <random>

namespace step_typetype
{
	namespace game
	{
		Stage::Stage( const std::size_t max_length ) : 
			mLetter_Min( 65 ) // 'A'
			, mLetter_Max( 90 ) // 'Z'
			, mLetters( max_length, mLetter_Min )
			, mIndicator_Current( 0 )
			, mIndicator_End( 1 )
		{}

		void Stage::Reset( const std::size_t length )
		{
			mLength = std::min( std::max( 1u, length ), mLetters.capacity() );

			mIndicator_Current = 0;
			mIndicator_End = mLength;

			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> dist( mLetter_Min, mLetter_Max );

			for( std::size_t i = 0; i < mLength; ++i )
			{
				mLetters[i] = static_cast<char>( dist( randomEngine ) );
			}
		}

		char Stage::GetLetter( const std::size_t target_pos ) const
		{
			assert( mLetters.size() > target_pos );

			return mLetters[target_pos];
		}

		bool Stage::RequestRemoveLetter( const char letter )
		{
			if( letter != mLetters[mIndicator_Current] )
			{
				return false;
			}

			++mIndicator_Current;

			return true;
		}

		bool Stage::IsGameClear() const
		{
			return mIndicator_End <= mIndicator_Current;
		}
	}
}
