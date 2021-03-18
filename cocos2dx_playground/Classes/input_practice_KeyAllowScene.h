#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "cpg_input_AllowedKeys.h"

#include "helper_BackToThePreviousScene.h"

namespace input_practice
{
	class KeyAllowScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		KeyAllowScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Key Allow"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

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

		cpg_input::AllowedKeys::Container mAllowedKeys;
	};
}
