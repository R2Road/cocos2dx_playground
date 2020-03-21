#pragma once

#include "2d/CCNode.h"

#include "Step01_Game_Terrain_Constant.h"

namespace step_pathfinder
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

				virtual Node* MakeTile( const step_pathfinder::game::terrain::TileData& tile_data, const int grid_x, const int grid_y );
				void UpdateTile( cocos2d::Node* tile_node, const step_pathfinder::game::terrain::eTileType tile_type );

			public:
				void UpdateTile( const int grid_x, const int grid_y, const step_pathfinder::game::terrain::eTileType tile_type );
				cocos2d::Vec2 ConvertPoint2Position( const int tx, const int ty ) const;

			protected:
				const int mWidth;
				const int mHeight;
				cocos2d::Size mTileSize;
				cocos2d::Vec2 mPivotPosition;
			};
		}
	}
}
