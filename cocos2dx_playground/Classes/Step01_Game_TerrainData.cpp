#include "Step01_Game_TerrainData.h"

#include <random>

namespace step01
{
	namespace game
	{
		TerrainData::TerrainData() :
			mWidth( 0 )
			, mHeight( 0 )
			, container()
		{}

		void TerrainData::load( const int width, const int height )
		{
			mWidth = width;
			mHeight = height;
			container.resize( mWidth * mHeight, false );

			//
			// generate dummy data
			//
			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> dist( 0, 1 );

			for( auto& d : container )
			{
				d = dist( randomEngine );
			}
		}

		const bool TerrainData::get( const std::size_t x, const std::size_t y ) const
		{
			const auto target_linear_index = x + ( y * mHeight );
			if( container.size() <= target_linear_index )
				return false;

			return container[target_linear_index];
		}
	} // namespace game
}
