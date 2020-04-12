#pragma once

#include "2d/CCNode.h"
#include "ui/UIWidget.h"
#include "math/CCGeometry.h"

namespace step_mole
{
	namespace game
	{
		struct StageConfig
		{
			const int BlockCount_Horizontal = 8;
			const int BlockCount_Vercital = 6;
			const cocos2d::Size BlockSize = cocos2d::Size( 30.f, 30.f );
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
			cocos2d::Node* MakeObject( const int object_tag, const cocos2d::Vec2 object_position );
			void onStageClick( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type );

		private:
			const StageConfig mStageConfig;
		};
	}
}
