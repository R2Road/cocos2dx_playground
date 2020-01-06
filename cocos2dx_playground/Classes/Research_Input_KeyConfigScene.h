#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"

#include "CPG_Input_AllowedKeys.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"

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

			void onEnter() override;
			void onExit() override;

		private:
			void onKeyConfigControl( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );

			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

			void onExitButton( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );
			void update_forExit( float dt );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;

			cpg::input::AllowedKeys::Container allowed_keys;
			cpg::InputTest::KeyMapConfigHelper keymap_config_helper;
			cocos2d::Node* current_button_node;
		};
	}
}
