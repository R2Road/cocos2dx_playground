#pragma once

#include <vector>

namespace step01
{
	namespace tool
	{
		class TerrainData
		{
		public:
			using Row = std::vector<int>;
			using Container = std::vector<Row>;

			TerrainData();

			void reSize( const std::size_t width, const std::size_t height );
			void fill( const Row::value_type tile_index );
			int getWidth() const { return mWidth; }
			int getHeight() const { return mHeight; }

			Row::value_type get( const std::size_t x, const std::size_t y ) const;
			void set( const std::size_t x, const std::size_t y, const Row::value_type tile_index );
			const Container& get() const { return mContainer; }

		private:
			int mWidth;
			int mHeight;
			Container mContainer;
		};
	}
}
