#pragma once

#include <string>

#include "ui/UIWidget.h"

#include "cpg_GridIndexConverter.h"

namespace step_defender
{
	namespace tool
	{
		class TileSheetNode : public cocos2d::Node
		{
		public:
			struct Config
			{
				int TileWidth = 32;
				int TileHeight = 32;

				std::string TexturePath;
			};

			struct Point
			{
				int x = 0;
				int y = 0;
			};

		private:
			TileSheetNode( const Config& config );

		public:
			static TileSheetNode* create( const Config& config );

		private:
			bool init() override;

			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			Point calculateSelectedPoint( cocos2d::Vec2 world_position );
			void updateIndicatorPosition( cocos2d::Vec2 world_position );

		private:
			const Config mConfig;
			const cpg::GridIndexConverter mGridIndexConverter;
			cocos2d::Node* mIndicator;
			Point mLastSelectedPoint;
		};
	}
}
