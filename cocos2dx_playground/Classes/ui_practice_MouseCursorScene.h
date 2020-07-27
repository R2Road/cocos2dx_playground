#pragma once

#include "2d/CCScene.h"

#include "helper_BackToThePreviousScene.h"

namespace ui_practice
{
	class MouseCursorScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	private:
		MouseCursorScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

	public:
		static const char* getTitle() { return "Mouse Cursor"; }
		static cocos2d::Scene* create( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		bool init() override;
		void onEnter() override;
		void onExit() override;

	private:
		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
