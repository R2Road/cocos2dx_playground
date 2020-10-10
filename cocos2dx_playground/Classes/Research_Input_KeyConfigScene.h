#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "CPG_Input_AllowedKeys.h"
#include "CPG_InputTest_KeyMapConfigHelper.h"

namespace research
{
	namespace input
	{
		class KeyConfigScene : public cocos2d::Scene
		{
		private:
			KeyConfigScene();

		public:
			static cocos2d::Scene* create();

		private:
			bool init() override;

		public:
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyConfigControl( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

			void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

			void onExitButton( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void update_forExit( float dt );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			cpg::input::AllowedKeys::Container mAllowedKeys;
			cpg::input_test::KeyMapConfigHelper mKeymapConfigHelper;
			cocos2d::Node* mCurrentButtonNode;
		};
	}
}
