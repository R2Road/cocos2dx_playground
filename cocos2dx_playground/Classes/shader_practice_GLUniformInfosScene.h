#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

#include "helper_BackToThePreviousScene.h"

namespace shader_practice
{
	class GLUniformInfosScene : public cocos2d::Scene, private helper::BackToThePreviousScene
	{
	public:
		GLUniformInfosScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback );

		static const char* getTitle() { return "GL Uniform List"; }
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
