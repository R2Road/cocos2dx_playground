#include "step_flipflip_game_StageData.h"

#include <chrono>
#include <cstdlib> // div
#include <random>

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		StageData::StageData() : mIndexConverter( 1, 1 ), mContainer()
		{}

		eCardType StageData::Get( const int x, const int y ) const
		{
			return mContainer[mIndexConverter.To_Linear( x, y )];
		}

		bool StageData::Reset( const int width, const int height, const int shuffle_count )
		{
			CCASSERT( 0 < width, "0 >= width" );
			CCASSERT( 0 < height, "0 >= height" );

			//
			// Check Odd Number
			//
			const int container_size = width * height;
			CCASSERT( 0 == ( container_size & 1 ), "odd number" );

			//
			// Index Converter
			//
			mIndexConverter = cpg::GridIndexConverter( width, height );

			//
			// Container Setup
			//
			mContainer.resize( container_size );

			//
			// Fill
			//
			int current_card_type = static_cast<int>( eCardType::FIRST );
			const int end_card_type = static_cast<int>( eCardType::SIZE );
			std::size_t i = 0;
			while( container_size > i )
			{
				mContainer[i] = static_cast<eCardType>( current_card_type );
				++i;
				mContainer[i] = static_cast<eCardType>( current_card_type );
				++i;

				++current_card_type;
				if( end_card_type == current_card_type )
				{
					current_card_type = static_cast<int>( eCardType::FIRST );
				}
			}

			// Shuffle x 2
			if( 0 < shuffle_count )
			{
				const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine random_engine( seed );

				for( int i = 0; shuffle_count > i; ++i )
				{
					std::shuffle( mContainer.begin(), mContainer.end(), random_engine );
				}
			}

			return true;
		}
	}
}
