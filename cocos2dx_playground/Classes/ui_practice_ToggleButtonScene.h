#pragma once

#include "2d/CCScene.h"
#include "extensions/GUI/CCControlExtension/CCControl.h"

#include "helper_BackToThePreviousScene.h"

namespace ui_practice
{
	class ToggleButtonScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	public:
		ToggleButtonScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		static const char* getTitle() { return "Toggle Button"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		void onToggleButton( cocos2d::Ref* sender, cocos2d::extension::Control::EventType event_type );
		void updateStatusView( const bool new_status );

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
