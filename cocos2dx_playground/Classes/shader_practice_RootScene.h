#pragma once

#include "2d/CCScene.h"

namespace shader_practice
{
	class RootScene : public cocos2d::Scene
	{
	private:
		RootScene();

	public:
		static const char* getTitle() { return "Shader Practice"; }
		static cocos2d::Scene* create();

		void onEnter() override;
		void onExit() override;

		void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event );

	private:
		cocos2d::EventListenerKeyboard* mKeyboardListener;
	};
}
