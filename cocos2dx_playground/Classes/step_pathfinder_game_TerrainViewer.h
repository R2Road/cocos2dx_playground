#pragma once

#include "2d/CCNode.h"

#include "step_pathfinder_game_terrain_Constant.h"

namespace step_pathfinder
{
	namespace game
	{
		struct TileData;

		class TerrainViewer : public cocos2d::Node
		{
		protected:
			explicit TerrainViewer( const int width, const int height, const cocos2d::Size tile_size, const cocos2d::Vec2 pivot_position );

		public:
			static TerrainViewer* create( const int width, const int height );

		protected:
			bool init() override;

			virtual cocos2d::Node* MakeTile( const TileData& tile_data, const int grid_x, const int grid_y );
			void UpdateTile( cocos2d::Node* tile_node, const eTileType tile_type );

		public:
			void UpdateTile( const int grid_x, const int grid_y, const eTileType tile_type );
			cocos2d::Vec2 ConvertPoint2Position( const int tx, const int ty ) const;

		protected:
			const int mWidth;
			const int mHeight;
			cocos2d::Size mTileSize;
			cocos2d::Vec2 mPivotPosition;
		};
	}
}
