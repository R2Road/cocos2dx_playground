#pragma once

#include <functional>

#include "ui/UIWidget.h"

#include "step_pathfinder_game_TerrainViewer.h"

namespace step_pathfinder
{
	namespace tool
	{
		class TerrainEditHelper : public step_pathfinder::game::TerrainViewer
		{
		public:
			using TileSelectCallback = std::function<void( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type )>;

		private:
			explicit TerrainEditHelper(
				const int width, const int height
				, const cocos2d::Size tile_size
				, const float tile_scale
				, const TileSelectCallback& tile_select_callback
			);

		public:
			static TerrainEditHelper* create( const int width, const int height, const cocos2d::Size tile_size, const TileSelectCallback& tile_select_callback );

		private:
			cocos2d::Node* MakeTile( const step_pathfinder::game::TileData& tile_data, const int grid_x, const int grid_y ) override;

			const TileSelectCallback mTileSelectCallback;
		};
	}
}
