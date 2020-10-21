#pragma once

#include <functional>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "step_pathfinder_game_Constant.h"

namespace step_pathfinder
{
	namespace tool_ui
	{
		class TileSelectNode : public cocos2d::Node
		{
		public:
			using TileSelectCallback = std::function<void( step_pathfinder::game::eTileType )>;

		private:
			TileSelectNode( const TileSelectCallback& tile_select_callback );

		public:
			static TileSelectNode* create( const TileSelectCallback& tile_select_callback );

		private:
			bool init() override;

			cocos2d::Node* makeMenuButton(
				const cocos2d::Size menu_size
				, const step_pathfinder::game::eTileType tile_type
				, const char* button_text
				, const cocos2d::ui::Widget::ccWidgetTouchCallback& callback
			) const;

		public:
			void SetIndicator( const step_pathfinder::game::eTileType tile_type );

		private:
			void onSelect( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const TileSelectCallback mTileSelectCallback;
		};
	}
}
