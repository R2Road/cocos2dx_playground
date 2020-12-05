#pragma once

#include <functional>
#include <string>

#include "ui/UIWidget.h"

#include "cpg_Position2GridIndexConverter.h"
#include "step_defender_game_Constant.h"

NS_CC_BEGIN
class Sprite;
NS_CC_END

namespace step_defender
{
	namespace tool
	{
		class TileSheetNode : public cocos2d::Node
		{
		public:
			using SelectCallback = std::function<void( int x, int y )>;

		private:
			struct Point
			{
				int x = 0;
				int y = 0;
			};

			TileSheetNode( const game::TileSheetConfiguration& config );

		public:
			static TileSheetNode* create( const game::TileSheetConfiguration& config );

		private:
			bool init() override;

			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void updateSelectedPoint( const cocos2d::Vec2& world_position );
			void updateIndicatorPosition();

		public:
			void SetSelectCallback( const SelectCallback& callback ) { mSelectCallback = callback; }

		private:
			const game::TileSheetConfiguration mConfig;
			const cpg::Position2GridIndexConverter mPosition2GridIndexConverter;
			SelectCallback mSelectCallback;

			cocos2d::Node* mIndicator;
			Point mLastSelectedPoint;
		};
	}
}
