#pragma once

#include "2d/CCNode.h"

#include "cpg_GridIndexConverter.h"
#include "step_pathfinder_game_Constant.h"

namespace step_pathfinder
{
	namespace game
	{
		class TerrainData;

		class TerrainViewer : public cocos2d::Node
		{
		private:
			explicit TerrainViewer( const int width, const int height, const cocos2d::Size tile_size, const float tile_scale );

		public:
			static TerrainViewer* create( const int width, const int height, const cocos2d::Size tile_size );

		private:
			bool init() override;

		public:
			void LoadTerrainData4Game( const TerrainData& terrain_data );
			void LoadTerrainData4Original( const TerrainData& terrain_data );

			void UpdateTile( const int grid_x, const int grid_y, const eTileType tile_type );
			cocos2d::Vec2 ConvertPoint2Position( const int grid_x, const int grid_y ) const;

		private:
			const int mWidth;
			const int mHeight;
			const cocos2d::Size mTileSize;
			const float mTileScale;
			const cpg::GridIndexConverter mGridIndexConverter;
		};
	}
}
