#pragma once

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

namespace step_mole
{
	namespace game
	{
		struct StageConfig
		{
			const int BlockCount_Horizontal = 8;
			const int BlockCount_Vercital = 6;
		};

		struct StageViewConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageView : public cocos2d::Node
		{
		private:
			StageView( const StageConfig stage_config );

		public:
			static StageView* create( const StageConfig stage_config, const StageViewConfig config = StageViewConfig() );

			bool init( const StageViewConfig config );

		private:
			void onStageClick( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const StageConfig mStageConfig;
		};
	}
}
