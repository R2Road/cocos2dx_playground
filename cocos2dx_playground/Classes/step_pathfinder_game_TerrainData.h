#pragma once

#include <vector>

#include "step_pathfinder_game_Constant.h"

namespace step_pathfinder
{
	namespace game
	{
		class TerrainData
		{
		public:
			using ValueT = eTileType;
			using RowT = std::vector<ValueT>;
			using ContainerT = std::vector<RowT>;

		public:
			TerrainData();

			ContainerT::const_iterator begin() const { return mContainer.begin(); }
			ContainerT::const_iterator end() const { return mContainer.end(); }

			bool Load( const char* file_name );

			int GetWidth() const { return mWidth; }
			int GetHeight() const { return mHeight; }

			ValueT Get( const int x, const int y ) const;
			void Set( const int x, const int y, const ValueT tile_type );

			TerrainPoint FindTilePoint( const ValueT tile_type ) const;
			bool isExist( const ValueT tile_type ) const;
			bool isIn( const int x, const int y ) const;

		private:
			const int mWidth;
			const int mHeight;
			ContainerT mContainer;
		};
	} // namespace game
}
