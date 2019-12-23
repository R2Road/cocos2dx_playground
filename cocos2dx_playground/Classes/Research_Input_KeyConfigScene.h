#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"

#include "CPG_Input_KeyMapConfigHelper.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using KeyCollectorSp = std::shared_ptr<class iKeyCollector>;
	}
}

namespace Research
{
	namespace Input
	{
		class KeyConfigScene : public cocos2d::Scene
		{
		public:
			KeyConfigScene() = default;

			static cocos2d::Scene* create();

			void update( float dt ) override;

		private:
			void onKeyConfigControl( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );

		private:
			CPG::Input::KeyCollectorSp input_collector;
			CPG::Input::KeyMapConfigHelper keymap_config_helper;
		};
	}
}
