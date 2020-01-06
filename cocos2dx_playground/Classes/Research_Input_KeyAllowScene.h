#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "CPG_Input_AllowedKeys.h"

namespace research
{
	namespace Input
	{
		class KeyAllowScene : public cocos2d::Scene
		{
		public:
			KeyAllowScene();

			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateForExit( float _dt );
			void onKeyAllowControl( cocos2d::Ref* _sender, cocos2d::ui::Widget::TouchEventType _touch_event_type );
			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*_event*/ );

		private:
			cocos2d::EventListenerKeyboard* keyboard_listener;
			bool go_exit;
			cpg::input::AllowedKeys::Container allowed_keys;
		};
	}
}
