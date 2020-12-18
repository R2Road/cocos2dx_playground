#include "step_flipflip_game_StageData.h"

#include <cstdlib> // div

USING_NS_CC;

namespace step_flipflip
{
	namespace game
	{
		StageData::StageData() : mContainer()
		{}

		bool StageData::Reset( const int width, const int height )
		{
			//
			// Check Odd Number
			//
			const int container_size = width * height;
			CCASSERT( 0 == ( container_size & 1 ), "odd number" );

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

			return true;
		}
	}
}
