#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"

#include "CPG_Input_KeyMapConfigHelper.h"

namespace Research
{
	namespace Input
	{
		class KeyConfigScene : public cocos2d::Scene
		{
		private:
			KeyConfigScene();

		public:
			static cocos2d::Scene* create();

		private:
			void onKeyConfigControl( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );

			void onExit( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );
			void update_forExit( float dt );

		private:
			CPG::Input::KeyMapConfigHelper keymap_config_helper;
			cocos2d::Node* current_button_node;
		};
	}
}
