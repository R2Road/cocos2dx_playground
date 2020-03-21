#pragma once

#include <functional>

#include "ui/UIWidget.h"

#include "step_pathfinder_game_terrain_Viewer.h"

namespace step_pathfinder
{
	namespace tool
	{
		class TerrainViewer : public step_pathfinder::game::terrain::Viewer
		{
		public:
			using TileSelectCallback = std::function<void( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type )>;

		private:
			explicit TerrainViewer(
				const int width, const int height
				, const cocos2d::Size tile_size
				, const cocos2d::Vec2 pivot_position
				, const TileSelectCallback& tile_select_callback
			);

		public:
			static TerrainViewer* create( const int width, const int height, const TileSelectCallback& tile_select_callback );

		private:
			cocos2d::Node* MakeTile( const step_pathfinder::game::terrain::TileData& tile_data, const int grid_x, const int grid_y ) override;

			const TileSelectCallback mTileSelectCallback;
		};
	}
}
