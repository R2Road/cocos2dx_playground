#pragma once

#include <functional>
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
			using SelectCallback = std::function<void( int x, int y )>;

			struct TileSheetConfiguration
			{
				int TileWidth = 32;
				int TileHeight = 32;

				int TileMargin_Width = 1;
				int TileMargin_Height = 1;

				std::string TexturePath;
			};

		private:
			struct Point
			{
				int x = 0;
				int y = 0;
			};

			TileSheetNode( const TileSheetConfiguration& config );

		public:
			static TileSheetNode* create( const TileSheetConfiguration& config );

		private:
			bool init() override;

			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void updateSelectedPoint( const cocos2d::Vec2& world_position );
			void updateIndicatorPosition();

		public:
			void SetSelectCallback( const SelectCallback& callback ) { mSelectCallback = callback; }
			cocos2d::Rect ConvertTilePoint2Rect( const int x, const int y ) const;

		private:
			const TileSheetConfiguration mConfig;
			const cpg::GridIndexConverter mGridIndexConverter;

			SelectCallback mSelectCallback;

			cocos2d::Node* mIndicator;
			Point mLastSelectedPoint;
		};
	}
}
