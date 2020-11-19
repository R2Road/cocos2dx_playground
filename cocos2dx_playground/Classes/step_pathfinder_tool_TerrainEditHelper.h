#pragma once

#include <functional>

#include "ui/UIWidget.h"

#include "cpg_GridIndexConverter.h"

namespace step_pathfinder
{
	namespace tool
	{
		class TerrainEditHelper : public cocos2d::Node
		{
		public:
			using TileSelectCallback = std::function<void( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type )>;

		private:
			explicit TerrainEditHelper( const int width, const int height, const TileSelectCallback& tile_select_callback );

		public:
			static TerrainEditHelper* create( const int width, const int height, const cocos2d::Size tile_size, const TileSelectCallback& tile_select_callback );

		private:
			bool init( const int width, const int height, const cocos2d::Size tile_size );

		private:
			const TileSelectCallback mTileSelectCallback;
			const cpg::GridIndexConverter mGridIndexConverter;
		};
	}
}
