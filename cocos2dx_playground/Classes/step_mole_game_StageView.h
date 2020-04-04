#pragma once

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	namespace game
	{
		struct StageViewConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageView : public cocos2d::Node
		{
		private:
			StageView( const int block_count_horizontal, const int block_count_vertical );

		public:
			static StageView* create( const int block_count_horizontal, const int block_count_vertical, const StageViewConfig config = StageViewConfig() );

			bool init( const StageViewConfig config );

		private:
			void onStageClick( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const int mBlockCount_Horizontal;
			const int mBlockCount_Vertical;
		};
	}
}
