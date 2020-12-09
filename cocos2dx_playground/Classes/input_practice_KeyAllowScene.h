#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "CPG_Input_AllowedKeys.h"

namespace input_practice
{
	class KeyAllowScene : public cocos2d::Scene
	{
	private:
		KeyAllowScene();

	public:
		static cocos2d::Scene* create();

	private:
		bool init() override;

	public:
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyReleased( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );
		void onKeyAllowControl( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;

		cpg::input::AllowedKeys::Container mAllowedKeys;
	};
}
