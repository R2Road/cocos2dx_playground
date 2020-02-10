#include "Step01_Game_TerrainData.h"

#include <random>

namespace step01
{
	namespace game
	{
		const int map_size_width = 5;
		const int map_size_height = 5;

		TerrainData::TerrainData() :
			mWidth( map_size_width )
			, mHeight( map_size_height )
			, mContainer()
		{
			mContainer.resize( mHeight );
			for( auto& r : mContainer )
			{
				r.resize( mWidth, step01::game::terrain::eTileType::damage );
			}
		}

		void TerrainData::load( const char* /*file_name*/ )
		{
			//
			// generate dummy data
			//
			std::random_device rd;
			std::mt19937 randomEngine( rd() );
			std::uniform_int_distribution<> dist( static_cast<int>( step01::game::terrain::eTileType::FIRST ), static_cast<int>( step01::game::terrain::eTileType::SIZE ) - 1 );

			for( auto& r : mContainer )
			{
				for( auto& t : r )
				{
					t = static_cast<step01::game::terrain::eTileType>( dist( randomEngine ) );
				}
			}
		}

		TerrainData::Row::value_type TerrainData::get( const std::size_t x, const std::size_t y ) const
		{
			if( mContainer.size() <= y )
			{
				return step01::game::terrain::eTileType::damage;
			}

			if( mContainer[y].size() <= x )
			{
				return step01::game::terrain::eTileType::damage;
			}

			return mContainer[y][x];
		}
		void TerrainData::set( const std::size_t x, const std::size_t y, const Row::value_type tile_type )
		{
			if( mContainer.size() <= y )
			{
				return;
			}

			if( mContainer[y].size() <= x )
			{
				return;
			}

			mContainer[y][x] = tile_type;
		}
	} // namespace game
}
