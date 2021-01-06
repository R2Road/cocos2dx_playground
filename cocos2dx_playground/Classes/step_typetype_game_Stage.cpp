#include "step_typetype_game_Stage.h"

#include <algorithm>
#include <cassert>
#include <random>

namespace step_typetype
{
	namespace game
	{
		Stage::Stage( const std::size_t max_length ) : 
			mLetters( max_length, 'A' )
			, mIndicator_Current( 0u )
			, mIndicator_End( 1u )
		{}

		void Stage::Reset( const std::size_t length )
		{
			mLength = std::min( std::max( 1u, length ), mLetters.capacity() );

			mIndicator_Current = 0u;
			mIndicator_End = mLength;

			const std::vector<char> LeftHand = {
				'Q', 'W', 'E', 'R', 'T'
				, 'A', 'S', 'D', 'F', 'G'
				, 'Z', 'X', 'C', 'V'
			};
			const std::vector<char> RightHand = {
				'Y', 'U', 'I', 'O', 'P'
				, 'H', 'J', 'K', 'L'
				, 'B', 'N', 'M',
			};

			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> left_dist( 0, LeftHand.size() - 1 );
			std::uniform_int_distribution<> right_dist( 0, RightHand.size() - 1 );

			for( std::size_t i = 0u; i < mLength; ++i )
			{
				if( 1u == i % 2u ) // odd number
				{
					mLetters[i] = static_cast<char>( LeftHand[left_dist( randomEngine )] );
				}
				else
				{
					mLetters[i] = static_cast<char>( RightHand[right_dist( randomEngine )] );
				}
			}
		}

		char Stage::GetLetter( const std::size_t target_pos ) const
		{
			assert( mLetters.size() > target_pos );

			return mLetters[target_pos];
		}

		bool Stage::RequestLetterDie( const char letter )
		{
			if( letter != mLetters[mIndicator_Current] )
			{
				return false;
			}

			++mIndicator_Current;

			return true;
		}

		bool Stage::IsStageClear() const
		{
			return mIndicator_End <= mIndicator_Current;
		}
	}
}
