#pragma once

#include <vector>

#include "step_pathfinder_game_Constant.h"

namespace step_pathfinder
{
	namespace game
	{
		struct TerrainPoint
		{
			int x = 0;
			int y = 0;
		};

		class TerrainData
		{
		public:
			using Row = std::vector<eTileType>;
			using Container = std::vector<Row>;

			TerrainData();
			virtual ~TerrainData() = default;

			bool load( const char* file_name );
			bool load();

			int GetWidth() const { return mWidth; }
			int GetHeight() const { return mHeight; }

			Row::value_type get( const int x, const int y ) const;
			void set( const int x, const int y, const Row::value_type tile_type );
			const Container& get() const { return mContainer; }

			TerrainPoint getPoint( const Row::value_type tile_type ) const;
			bool isExist( const Row::value_type tile_type ) const;
			bool isIn( const int x, const int y ) const;

		protected:
			const int mWidth;
			const int mHeight;
			Container mContainer;
		};
	} // namespace game
}
