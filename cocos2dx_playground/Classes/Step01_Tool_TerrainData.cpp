#include "Step01_Tool_TerrainData.h"

namespace step01
{
	namespace tool
	{
		TerrainData::TerrainData() :
			mWidth( 0 )
			, mHeight( 0 )
			, mContainer()
		{}

		void TerrainData::reSize( const std::size_t width, const std::size_t height )
		{
			mWidth = width;
			mHeight = height;
			mContainer.resize( height );
			for( auto& r : mContainer )
			{
				r.resize( width, step01::game::terrain::eTileType::damage );
			}
		}
		void TerrainData::fill( const Row::value_type tile_type )
		{
			for( auto& r : mContainer )
			{
				for( auto& t : r )
				{
					t = tile_type;
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
	}
}
