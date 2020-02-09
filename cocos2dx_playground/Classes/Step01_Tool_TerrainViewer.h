#pragma once

#include <functional>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "Step01_Game_Terrain_Constant.h"

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			struct TileData;
		}
	}

	namespace tool
	{
		class TerrainViewer : public cocos2d::Node
		{
		public:
			using TileSelectCallback = std::function<void( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type )>;

		private:
			explicit TerrainViewer( const int width, const int height, const TileSelectCallback& tile_select_callback );

		public:
			static TerrainViewer* create( const int width, const int height, const TileSelectCallback& tile_select_callback );

		private:
			bool init() override;

			Node* MakeTile( const step01::game::terrain::TileData& tile_data, const int grid_x, const int grid_y );
			void UpdateTile( cocos2d::Node* tile_node, const step01::game::terrain::eTileType tile_type );

		public:
			void UpdateTile( const int grid_x, const int grid_y, const step01::game::terrain::eTileType tile_type );

		private:
			int mWidth;
			int mHeight;
			const TileSelectCallback& mTileSelectCallback;
		};
	}
}
