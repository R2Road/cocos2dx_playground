#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "CPG_Input_AllowedKeys.h"

namespace input_practice
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
		void updateForExit( float dt );
		void onKeyAllowControl( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
		void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
		bool mGoExit;
		cpg::input::AllowedKeys::Container mAllowedKeys;
	};
}
