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
			using ValueT = eTileType;
			using Row = std::vector<ValueT>;
			using ContainerT = std::vector<Row>;

			TerrainData();
			virtual ~TerrainData() = default;

			bool Load( const char* file_name );
			bool Load();

			int GetWidth() const { return mWidth; }
			int GetHeight() const { return mHeight; }

			ValueT Get( const int x, const int y ) const;
			void Set( const int x, const int y, const ValueT tile_type );

			TerrainPoint FindTilePoint( const ValueT tile_type ) const;
			bool isExist( const ValueT tile_type ) const;
			bool isIn( const int x, const int y ) const;

		protected:
			const int mWidth;
			const int mHeight;
			ContainerT mContainer;
		};
	} // namespace game
}
