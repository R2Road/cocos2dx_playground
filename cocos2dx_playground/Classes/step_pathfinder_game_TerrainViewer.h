#pragma once

#include "2d/CCNode.h"

#include "cpg_GridIndexConverter.h"
#include "step_pathfinder_game_Constant.h"

namespace step_pathfinder
{
	namespace game
	{
		struct TileData;

		class TerrainViewer : public cocos2d::Node
		{
		protected:
			explicit TerrainViewer( const int width, const int height, const cocos2d::Size tile_size, const float tile_scale );

		public:
			static TerrainViewer* create( const int width, const int height, const cocos2d::Size tile_size );

		private:
			bool init() override;

		public:
			void UpdateTile( const int grid_x, const int grid_y, const eTileType tile_type );
			cocos2d::Vec2 ConvertPoint2Position( const int tx, const int ty ) const;

		private:
			const int mWidth;
			const int mHeight;
			const cocos2d::Size mTileSize;
			const float mTileScale;
			const cpg::GridIndexConverter mGridIndexConverter;
		};
	}
}
