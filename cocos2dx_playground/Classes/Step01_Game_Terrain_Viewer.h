#pragma once

#include "2d/CCNode.h"

#include "Step01_Game_Terrain_Constant.h"

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			struct TileData;

			class Viewer : public cocos2d::Node
			{
			protected:
				explicit Viewer( const int width, const int height, const cocos2d::Size tile_size, const cocos2d::Vec2 pivot_position );

			public:
				static Viewer* create( const int width, const int height );

			protected:
				bool init() override;

				virtual Node* MakeTile( const step01::game::terrain::TileData& tile_data, const int grid_x, const int grid_y );
				void UpdateTile( cocos2d::Node* tile_node, const step01::game::terrain::eTileType tile_type );

			public:
				void UpdateTile( const int grid_x, const int grid_y, const step01::game::terrain::eTileType tile_type );

			protected:
				const int mWidth;
				const int mHeight;
				cocos2d::Size mTileSize;
				cocos2d::Vec2 mPivotPosition;
			};
		}
	}
}
