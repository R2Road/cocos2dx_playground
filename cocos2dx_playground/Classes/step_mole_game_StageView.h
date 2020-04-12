#pragma once

#include <vector>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"
#include "math/CCGeometry.h"

namespace step_mole
{
	class ObjectComponent;

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

		private:
			bool init( const StageViewConfig config );

			cocos2d::Node* MakeObject( const int object_tag, const cocos2d::Vec2 object_position, const int defalut_view_type );
			void onStageClick( cocos2d::Ref* /*sender*/, cocos2d::ui::Widget::TouchEventType touch_event_type );

		public:
			void RequestAction( const std::size_t object_index, const float life_time );

		private:
			const StageConfig mStageConfig;

			std::vector<ObjectComponent*> mObjectComponentList;
		};
	}
}
