#pragma once

#include <string>

#include "ui/UIWidget.h"

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

		private:
			TileSheetNode( const Config& config );

		public:
			static TileSheetNode* create( const Config& config );

		private:
			bool init() override;

			void onButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const Config mConfig;
			cocos2d::Node* mIndicator;
		};
	}
}
