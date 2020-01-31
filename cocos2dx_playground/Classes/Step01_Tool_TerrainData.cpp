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
				r.resize( width, 0 );
			}
		}
		void TerrainData::fill( const Row::value_type tile_index )
		{
			for( auto& r : mContainer )
			{
				for( auto& t : r )
				{
					t = tile_index;
				}
			}
		}

		TerrainData::Row::value_type TerrainData::get( const std::size_t x, const std::size_t y ) const
		{
			if( mContainer.size() <= y )
			{
				return 0;
			}

			if( mContainer[y].size() <= x )
			{
				return 0;
			}

			return mContainer[y][x];
		}
		void TerrainData::set( const std::size_t x, const std::size_t y, const Row::value_type tile_index )
		{
			if( mContainer.size() <= y )
			{
				return;
			}

			if( mContainer[y].size() <= x )
			{
				return;
			}

			mContainer[y][x] = tile_index;
		}
	}
}
