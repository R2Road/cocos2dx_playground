#pragma once

#include <vector>

#include "Step01_Game_Terrain_Constant.h"

namespace step01
{
	namespace tool
	{
		class TerrainData
		{
		public:
			using Row = std::vector<step01::game::terrain::eTileType>;
			using Container = std::vector<Row>;

			TerrainData();

			void reSize( const std::size_t width, const std::size_t height );
			void fill( const Row::value_type tile_type );
			int getWidth() const { return mWidth; }
			int getHeight() const { return mHeight; }

			Row::value_type get( const std::size_t x, const std::size_t y ) const;
			void set( const std::size_t x, const std::size_t y, const Row::value_type tile_type );
			const Container& get() const { return mContainer; }

			void save( const char* file_name );
			bool load( const char* file_name );

		private:
			int mWidth;
			int mHeight;
			Container mContainer;
		};
	}
}
